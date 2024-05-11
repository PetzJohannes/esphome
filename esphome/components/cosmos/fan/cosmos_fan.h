#pragma once

#include <set>

#include "esphome/core/automation.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace cosmos {

class CosmosFan : public Component, public fan::Fan {
 public:
  CosmosFan(int speed_count) : speed_count_(speed_count) {}

  void set_pin_a(output::FloatOutput *pin_a) { pin_a_ = pin_a; }
  void set_pin_b(output::FloatOutput *pin_b) { pin_b_ = pin_b; }
  void set_brake_start_level(float brake_start_level) { brake_start_level_ = brake_start_level; }
  void set_brake_end_level(float brake_end_level) { brake_end_level_ = brake_end_level; }
  void set_brake_stop_level(float brake_stop_level) { brake_stop_level_ = brake_stop_level; }

  void setup() override;
  void dump_config() override;
  fan::FanTraits get_traits() override { return this->traits_; }

  fan::FanCall brake();

 protected:
  output::FloatOutput *pin_a_;
  output::FloatOutput *pin_b_;
  float brake_start_level_;
  float brake_end_level_;
  float brake_stop_level_;
  output::BinaryOutput *oscillating_{nullptr};
  int speed_count_{};
  fan::FanTraits traits_;

  void control(const fan::FanCall &call) override;
  void write_state_();

  void set_hbridge_levels_(float a_level, float b_level);

  float get_brake_stop_level_();
};

template<typename... Ts> class BrakeAction : public Action<Ts...> {
 public:
  explicit BrakeAction(CosmosFan *parent) : parent_(parent) {}

  void play(Ts... x) override { this->parent_->brake(); }

  CosmosFan *parent_;
};

}  // namespace cosmos
}  // namespace esphome