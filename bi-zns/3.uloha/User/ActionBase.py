from OrodaelTurrim.Business.Interface.Player import PlayerTag
from OrodaelTurrim.Business.Proxy import GameControlProxy
from ExpertSystem.Business.UserFramework import IActionBase
from OrodaelTurrim.Business.Logger import Logger
from OrodaelTurrim.Structure.Enums import GameObjectType, HexDirection
from OrodaelTurrim.Structure.Filter.AttackFilter import AttackStrongestFilter, AttackNoResistantFilter, \
    AttackLeastVisibleFilter
from OrodaelTurrim.Structure.Filter.Factory import FilterFactory
from OrodaelTurrim.Structure.GameObjects.GameObject import SpawnInformation
from OrodaelTurrim.Structure.Position import OffsetPosition
from User.AttackFilter import ProtectBaseAttackFilter, TwoClosestToBasePositionsAttackFilter, WhoICanDamageAttackFilter


class ActionBase(IActionBase):
    """
    You can define here your custom actions. Methods must be public (not starting with __ or _) and must have unique
    names. Methods could have as many arguments as you want. Instance of this class will be available in
    Inference class.

    **This class provides:**

    * self.game_control_proxy [GameControlProxy] for doing actions in game
    * self.player [PlayerTag] instance of your player for identification yourself in proxy

    Usage of ActionBase is described in documentation.
    """
    game_control_proxy: GameControlProxy
    player: PlayerTag

    def build_base(self, find_suitable_place_for_king):
        self.spawn_unit(GameObjectType.BASE, find_suitable_place_for_king)

    def build_archer(self, visible_free_tile):
        self.spawn_unit(GameObjectType.ARCHER, visible_free_tile)

    def place_knight_on_given_position(self, direction_from_base_is_empty):
        self.spawn_unit(GameObjectType.KNIGHT, direction_from_base_is_empty)

    def place_druid_on_given_position(self, direction_from_base_is_empty):
        self.spawn_unit(GameObjectType.DRUID, direction_from_base_is_empty)

    def place_magician_on_given_position(self, direction_from_base_is_empty):
        self.spawn_unit(GameObjectType.MAGICIAN, direction_from_base_is_empty)

    def place_archer_on_given_position(self, direction_from_base_is_empty):
        self.spawn_unit(GameObjectType.ARCHER, direction_from_base_is_empty)


    # placing knight - cyclop
    def place_knight_on_given_position_up_cyclop(self, cyclop_approaching_from_up):
        self.spawn_unit(GameObjectType.KNIGHT, cyclop_approaching_from_up)
    def place_knight_on_given_position_down_cyclop(self, cyclop_approaching_from_down):
        self.spawn_unit(GameObjectType.KNIGHT, cyclop_approaching_from_down)
    def place_knight_on_given_position_left_cyclop(self, cyclop_approaching_from_left):
        self.spawn_unit(GameObjectType.KNIGHT, cyclop_approaching_from_left)
    def place_knight_on_given_position_right_cyclop(self, cyclop_approaching_from_right):
        self.spawn_unit(GameObjectType.KNIGHT, cyclop_approaching_from_right)

    # placing druid - demon
    def place_druid_on_given_position_up_demon(self, demon_approaching_from_up):
        self.spawn_unit(GameObjectType.DRUID, demon_approaching_from_up)
    def place_druid_on_given_position_down_demon(self, demon_approaching_from_down):
        self.spawn_unit(GameObjectType.DRUID, demon_approaching_from_down)
    def place_druid_on_given_position_left_demon(self, demon_approaching_from_left):
        self.spawn_unit(GameObjectType.DRUID, demon_approaching_from_left)
    def place_druid_on_given_position_right_demon(self, demon_approaching_from_right):
        self.spawn_unit(GameObjectType.DRUID, demon_approaching_from_right)

    # placing knight - demon
    def place_knight_on_given_position_up_demon(self, demon_approaching_from_up):
        self.spawn_unit(GameObjectType.KNIGHT, demon_approaching_from_up)
    def place_knight_on_given_position_down_demon(self, demon_approaching_from_down):
        self.spawn_unit(GameObjectType.KNIGHT, demon_approaching_from_down)
    def place_knight_on_given_position_left_demon(self, demon_approaching_from_left):
        self.spawn_unit(GameObjectType.KNIGHT, demon_approaching_from_left)
    def place_knight_on_given_position_right_demon(self, demon_approaching_from_right):
        self.spawn_unit(GameObjectType.KNIGHT, demon_approaching_from_right)

    # placing druid - elemental
    def place_druid_on_given_position_up_elemental(self, elemental_approaching_from_up):
        self.spawn_unit(GameObjectType.DRUID, elemental_approaching_from_up)
    def place_druid_on_given_position_down_elemental(self, elemental_approaching_from_down):
        self.spawn_unit(GameObjectType.DRUID, elemental_approaching_from_down)
    def place_druid_on_given_position_left_elemental(self, elemental_approaching_from_left):
        self.spawn_unit(GameObjectType.DRUID, elemental_approaching_from_left)
    def place_druid_on_given_position_right_elemental(self, elemental_approaching_from_right):
        self.spawn_unit(GameObjectType.DRUID, elemental_approaching_from_right)

    # placing knight - gargoyle
    def place_knight_on_given_position_up_gargoyle(self, gargoyle_approaching_from_up):
        self.spawn_unit(GameObjectType.KNIGHT, gargoyle_approaching_from_up)
    def place_knight_on_given_position_down_gargoyle(self, gargoyle_approaching_from_down):
        self.spawn_unit(GameObjectType.KNIGHT, gargoyle_approaching_from_down)
    def place_knight_on_given_position_left_gargoyle(self, gargoyle_approaching_from_left):
        self.spawn_unit(GameObjectType.KNIGHT, gargoyle_approaching_from_left)
    def place_knight_on_given_position_right_gargoyle(self, gargoyle_approaching_from_right):
        self.spawn_unit(GameObjectType.KNIGHT, gargoyle_approaching_from_right)

    # placing druid - minotaur
    def place_druid_on_given_position_up_minotaur(self, minotaur_approaching_from_up):
        self.spawn_unit(GameObjectType.DRUID, minotaur_approaching_from_up)
    def place_druid_on_given_position_down_minotaur(self, minotaur_approaching_from_down):
        self.spawn_unit(GameObjectType.DRUID, minotaur_approaching_from_down)
    def place_druid_on_given_position_left_minotaur(self, minotaur_approaching_from_left):
        self.spawn_unit(GameObjectType.DRUID, minotaur_approaching_from_left)
    def place_druid_on_given_position_right_minotaur(self, minotaur_approaching_from_right):
        self.spawn_unit(GameObjectType.DRUID, minotaur_approaching_from_right)

    # placing archer - necromancer
    def place_archer_on_given_position_up_necromancer(self, necromancer_approaching_from_up):
        self.spawn_unit(GameObjectType.ARCHER, necromancer_approaching_from_up)
    def place_archer_on_given_position_down_necromancer(self, necromancer_approaching_from_down):
        self.spawn_unit(GameObjectType.ARCHER, necromancer_approaching_from_down)
    def place_archer_on_given_position_left_necromancer(self, necromancer_approaching_from_left):
        self.spawn_unit(GameObjectType.ARCHER, necromancer_approaching_from_left)
    def place_archer_on_given_position_right_necromancer(self, necromancer_approaching_from_right):
        self.spawn_unit(GameObjectType.ARCHER, necromancer_approaching_from_right)

    # placing archer - orc
    def place_archer_on_given_position_up_orc(self, orc_approaching_from_up):
        self.spawn_unit(GameObjectType.ARCHER, orc_approaching_from_up)
    def place_archer_on_given_position_down_orc(self, orc_approaching_from_down):
        self.spawn_unit(GameObjectType.ARCHER, orc_approaching_from_down)
    def place_archer_on_given_position_left_orc(self, orc_approaching_from_left):
        self.spawn_unit(GameObjectType.ARCHER, orc_approaching_from_left)
    def place_archer_on_given_position_right_orc(self, orc_approaching_from_right):
        self.spawn_unit(GameObjectType.ARCHER, orc_approaching_from_right)

    # placing archer - skeleton
    def place_archer_on_given_position_up_skeleton(self, skeleton_approaching_from_up):
        self.spawn_unit(GameObjectType.ARCHER, skeleton_approaching_from_up)
    def place_archer_on_given_position_down_skeleton(self, skeleton_approaching_from_down):
        self.spawn_unit(GameObjectType.ARCHER, skeleton_approaching_from_down)
    def place_archer_on_given_position_left_skeleton(self, skeleton_approaching_from_left):
        self.spawn_unit(GameObjectType.ARCHER, skeleton_approaching_from_left)
    def place_archer_on_given_position_right_skeleton(self, skeleton_approaching_from_right):
        self.spawn_unit(GameObjectType.ARCHER, skeleton_approaching_from_right)

    def build_ent_near_king(self, free_tile_near_king):
        self.spawn_unit(GameObjectType.ENT, free_tile_near_king)
    def build_archer_near_king(self, free_tile_near_king):
        self.spawn_unit(GameObjectType.ARCHER, free_tile_near_king)
    def build_knight_near_king(self, free_tile_near_king):
        self.spawn_unit(GameObjectType.KNIGHT, free_tile_near_king)
    def build_magician_near_king(self, free_tile_near_king):
        self.spawn_unit(GameObjectType.MAGICIAN, free_tile_near_king)
    def place_druid_near_king(self, base_in_danger):
        self.spawn_unit(GameObjectType.DRUID, base_in_danger)

    def place_knight_near_king_save_him(self, base_in_danger):
        self.spawn_unit(GameObjectType.KNIGHT, base_in_danger)

    def place_knight_near_king_low_health(self, base_low_on_health):
        self.spawn_unit(GameObjectType.KNIGHT, base_low_on_health)

    def spawn_unit(self, unit: GameObjectType, position):
        # AttackLeastVisibleFilter --> utoci na ty, kteri maji nejmensi pocet zivotu
        # AttackNoResistantFilter --> utoci na ty, kteri nejsou odolni vuci utokum aktualni jednotky
        # AttackStrongestFilter --> utoci na ty nejsilnejsi mozne jednotky
        protect_base_filter = FilterFactory().attack_filter(ProtectBaseAttackFilter)
        closest_to_base_filter = FilterFactory().attack_filter(TwoClosestToBasePositionsAttackFilter)
        who_i_can_damage_filter = FilterFactory().attack_filter(WhoICanDamageAttackFilter)
        # decide between these two filters
        lowest_on_hp_filter = FilterFactory().attack_filter(AttackLeastVisibleFilter)
        no_resistance_filter = FilterFactory().attack_filter(AttackNoResistantFilter)
        strongest_filter = FilterFactory().attack_filter(AttackStrongestFilter)

        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             unit,
                             position,
                             [who_i_can_damage_filter, protect_base_filter, no_resistance_filter,
                              closest_to_base_filter, lowest_on_hp_filter], []))
