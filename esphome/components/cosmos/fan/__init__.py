import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.automation import maybe_simple_id
from esphome.components import fan, output
from esphome.const import (
    CONF_ID,
    CONF_SPEED_COUNT,
    CONF_PIN_A,
    CONF_PIN_B,
)

CODEOWNERS = ["@PetzJohannes"]

CONF_BRAKE_START_LEVEL = 'brake_start_level'
CONF_BRAKE_STOP_LEVEL = 'brake_stop_level'
CONF_BRAKE_END_LEVEL = 'brake_end_level'

cosmos_ns = cg.esphome_ns.namespace("cosmos")
CosmosFan = cosmos_ns.class_("CosmosFan", cg.Component, fan.Fan)

# Actions
BrakeAction = cosmos_ns.class_("BrakeAction", automation.Action)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(CosmosFan),
        cv.Required(CONF_PIN_A): cv.use_id(output.FloatOutput),
        cv.Required(CONF_PIN_B): cv.use_id(output.FloatOutput),
        cv.Optional(CONF_SPEED_COUNT, default=100): cv.int_range(min=1),
        cv.Optional(CONF_BRAKE_START_LEVEL, default=41): cv.zero_to_one_float,
        cv.Optional(CONF_BRAKE_STOP_LEVEL): cv.zero_to_one_float,
        cv.Optional(CONF_BRAKE_END_LEVEL, default=47): cv.zero_to_one_float,
    }
).extend(cv.COMPONENT_SCHEMA)


@automation.register_action(
    "fan.cosmos.brake",
    BrakeAction,
    maybe_simple_id({cv.Required(CONF_ID): cv.use_id(CosmosFan)}),
)
async def fan_cosmos_brake_to_code(config, action_id, template_arg, args):
    print("COSMOS REGISTER ACTION")
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)

async def to_code(config):
    var = cg.new_Pvariable(
        config[CONF_ID],
        config[CONF_SPEED_COUNT],
        config[CONF_BRAKE_START_LEVEL],
        config[CONF_BRAKE_END_LEVEL],
    )
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    pin_a_ = await cg.get_variable(config[CONF_PIN_A])
    cg.add(var.set_pin_a(pin_a_))
    pin_b_ = await cg.get_variable(config[CONF_PIN_B])
    cg.add(var.set_pin_b(pin_b_))
    brake_start_level_ = await cg.get_variable(config[CONF_BRAKE_START_LEVEL])
    cg.add(var.set_brake_start_level(brake_start_level_))
    brake_end_level_ = await cg.get_variable(config[CONF_BRAKE_END_LEVEL])
    cg.add(var.set_brake_end_level(brake_end_level_))

    if CONF_BRAKE_STOP_LEVEL in config:
        brake_stop_level_ = await cg.get_variable(config[CONF_BRAKE_STOP_LEVEL])
        cg.add(var.set_brake_stop_level(brake_stop_level_))