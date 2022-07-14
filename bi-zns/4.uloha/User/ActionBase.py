from OrodaelTurrim.Business.Interface.Player import PlayerTag
from OrodaelTurrim.Business.Proxy import GameControlProxy
from ExpertSystem.Business.UserFramework import IActionBase
from OrodaelTurrim.Business.Logger import Logger
from OrodaelTurrim.Structure.Enums import GameObjectType, HexDirection, TerrainType
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

    def find_free_tile_near_king(self):
        border_tiles = self.map_proxy.get_border_tiles()
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        base_position = self.map_proxy.get_bases_positions().pop()
        positions_to_check = base_position.get_all_neighbours()
        while positions_to_check[0] not in border_tiles and positions_to_check[0] in visible_tiles:
            position = positions_to_check.pop(0)
            if not self.map_proxy.is_position_occupied(position):
                return position
            positions_to_check += position.get_all_neighbours()

        return None

    def spawn_archers_fuzzy(self, count):
        count_int = int(count)
        for i in range(count_int):
            available_place = self.find_free_tile_near_king()
            if available_place is not None:
                self.spawn_unit(GameObjectType.ARCHER, available_place)
