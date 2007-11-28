------------------------------------------------------------------------------[[
-- Filename: items.lua
--
-- Description: This file contains the definitions of all items that exist in
-- Hero of Allacrost. Each item has a unique integer identifier that is used
-- as its key in the items table below.
--
-- Note (1): Item ids do *not* need to be sequential. When you make a new item,
-- consider giving it some space from the other items. This way, we won't
-- get a jumbled mess of different ids.
--
-- Note (2): Valid ids for items are 1-10000. Do not exceed this limit, because
-- higher id values correspond to other types of objects (weapons, armor, etc.)
--
-- Note (3): Items can have three different types of attack targets: an attack
-- point, an actor, or a party. The script defined for each item assumes that
-- the argument which it is passed (GlobalAttackPoint, GlobalActor, GlobalParty)
-- is what it expected it to be.
------------------------------------------------------------------------------]]

-- All item definitions are stored in this table
if (items == nil) then
   items = {}
end

-- -----------------------------------------------------------------------------
-- IDs 1-1000 are reserved for healing potions
-- -----------------------------------------------------------------------------

items[1] = {
	name = "Healing Potion",
	description = "Restores a small amount of hit points to a target.",
	icon = "img/icons/items/health_potion.png",
	target_type = hoa_global.GameGlobal.GLOBAL_TARGET_ACTOR,
	target_ally = true,
	standard_price = 60,

	BattleUse = function(target, instigator)
		target:AddHitPoints(100);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end,

	MenuUse = function(target)
		target:AddHitPoints(100);
		AudioManager:PlaySound("snd/potion_drink.wav");
	end
}

-- -----------------------------------------------------------------------------
-- IDs 1001-2000 are reserved for status potions
-- -----------------------------------------------------------------------------

-- items[1001] = {}

-- -----------------------------------------------------------------------------
-- IDs 2001-3000 are reserved for elemental potions
-- -----------------------------------------------------------------------------

-- items[2001] = {}
