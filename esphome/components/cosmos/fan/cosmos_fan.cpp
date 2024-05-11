#include "cosmos_fan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cosmos {

static const char *const TAG = "fan.cosmos";

void CosmosFan::set_hbridge_levels_(float a_level, float b_level) {
  this->pin_a_->set_level(a_level);
  this->pin_b_->set_level(b_level);
  ESP_LOGD(TAG, "Setting speed: a: %.2f, b: %.2f", a_level, b_level);
}

fan::FanCall CosmosFan::brake() {
  ESP_LOGD(TAG, "Braking");
  this->set_hbridge_levels_(this->get_brake_stop_level_(), 1.0f);
  return this->make_call().set_state(false);
}

void CosmosFan::setup() {
  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(*this);
    this->write_state_();
  }

  // Construct traits
  this->traits_ = fan::FanTraits(this->oscillating_ != nullptr, true, true, this->speed_count_);
}

void CosmosFan::dump_config() {
  LOG_FAN("", "Cosmos Fan", this);
}

void CosmosFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value())
    this->state = *call.get_state();
  if (call.get_speed().has_value())
    this->speed = *call.get_speed();
  if (call.get_oscillating().has_value())
    this->oscillating = *call.get_oscillating();
  if (call.get_direction().has_value())
    this->direction = *call.get_direction();

  this->write_state_();
  this->publish_state();
}

void CosmosFan::write_state_() {
  float speed = this->state ? static_cast<float>(this->speed) / static_cast<float>(this->speed_count_) : 0.0f;
  
  if (speed == 0.0f) {  // off means idle
    this->set_hbridge_levels_(this->get_brake_stop_level_(), 1.0f);
  } else if (this->direction == fan::FanDirection::FORWARD) {
    this->set_hbridge_levels_(this->brake_start_level_ * (1.0f - speed), 1.0f);
  } else {  // fan::FAN_DIRECTION_REVERSE
    this->set_hbridge_levels_((speed * (1.0f - this->brake_end_level_)) + this->brake_end_level_, 1.0f);
  }

  if (this->oscillating_ != nullptr)
    this->oscillating_->set_state(this->oscillating);
}

float CosmosFan::get_brake_stop_level_() {
  if (this->brake_stop_level_ == -1.0f) {
    float brake_stop_level = (this->brake_end_level_ - this->brake_start_level_) / 2.0f;
    ESP_LOGD(TAG, "Brake stop level calculated: %.2f", brake_stop_level);
    return brake_stop_level;
  }

  ESP_LOGD(TAG, "Brake stop level is set to: %.2f", this->brake_stop_level_);
  return this->brake_stop_level_;
}

}  // namespace cosmos
}  // namespace esphome