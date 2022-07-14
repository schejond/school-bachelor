from OrodaelTurrim.Business.Interface.Player import IPlayer
from OrodaelTurrim.Business.Proxy import GameControlProxy, MapProxy
from ExpertSystem.Business.UserFramework import IActionBase
from OrodaelTurrim.Business.Logger import Logger
from OrodaelTurrim.Structure.Enums import GameObjectType, HexDirection, TerrainType
from OrodaelTurrim.Structure.Filter.AttackFilter import AttackStrongestFilter
from OrodaelTurrim.Structure.Filter.Factory import FilterFactory
from OrodaelTurrim.Structure.GameObjects.GameObject import SpawnInformation
from OrodaelTurrim.Structure.Position import OffsetPosition, CubicPosition
from User.AttackFilter import DummyAttackFilter, EmptyAttackFilter


class ActionBase(IActionBase):
    """
    You can define here your custom actions. Methods must be public (not starting with __ or _) and must have unique
    names. Methods could have as many arguments as you want. Instance of this class will be available in
    Interference class.


    **This class provides:**

    * self.game_control_proxy [GameControlProxy] for doing actions in game
    * self.map_proxy [MapProxy] for finding places on map
    * self.player [IPlayer] instance of your player for identification yourself in proxy

    MapProxy should be used there only for finding right place on the map. For example functions like:
    * spawn_knight_on_nearest_mountain(x,y)
    * spawn_unit_near_to_base(amount_of_units, unit_type)
    * spawn_unit_far_in_direction(direction)
    * etc...

    It is forbidden, to create whole login in those functions. Whole behaviour logic must be editable without,
    touching code in ActionBase (login must mainly depend on rules). So it's forbidden to use functions like:
    * prepare_defence()
    * spawn_ideal_amount_of_units_at_ideal_places()
    * defend_my_base()
    * etc...

    You can use () operator on ActionBase instance to call your function by `str` name or `Expression` class.
    Expression class will also pass arguments from self to your method. () operator using only args so be careful about
    order and number of arguments.

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !!               TODO: Write implementation of your actions HERE                !!
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    """
    game_control_proxy: GameControlProxy
    map_proxy: MapProxy
    player: IPlayer


    def build_base(self, position_q: int, position_r: int):
        # Custom log messages
        Logger.log('Building base')

        # Create instance of custom filter
        empty_filter = FilterFactory().attack_filter(EmptyAttackFilter)
        dummy_filter = FilterFactory().attack_filter(DummyAttackFilter, 'Base attacking')

        # Create instance of default filter
        strongest_filter = FilterFactory().attack_filter(AttackStrongestFilter)

        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.BASE,
                             OffsetPosition(int(position_q), int(position_r)),
                             [empty_filter, dummy_filter, strongest_filter], []))

    def place_knight_right_up_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        right_up_from_base_position = position_of_base.neighbour(HexDirection.RIGHT_UPPER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.KNIGHT,
                             right_up_from_base_position,
                             [], []))

    def place_knight_right_down_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        right_down_from_base_position = position_of_base.neighbour(HexDirection.RIGHT_LOWER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.KNIGHT,
                             right_down_from_base_position,
                             [], []))

    def place_knight_left_up_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        left_up_from_base_position = position_of_base.neighbour(HexDirection.LEFT_UPPER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.KNIGHT,
                             left_up_from_base_position,
                             [], []))

    def place_knight_left_down_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        left_down_from_base_position = position_of_base.neighbour(HexDirection.LEFT_LOWER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.KNIGHT,
                             left_down_from_base_position,
                             [], []))

    def place_magician_up_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        up_from_base_position = position_of_base.neighbour(HexDirection.UPPER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.MAGICIAN,
                             up_from_base_position,
                             [], []))

    def place_magician_down_from_base(self):
        position_of_base = self.map_proxy.get_bases_positions().pop().cubic
        down_from_base_position = position_of_base.neighbour(HexDirection.LOWER)
        self.game_control_proxy.spawn_unit(
            SpawnInformation(self.player,
                             GameObjectType.MAGICIAN,
                             down_from_base_position,
                             [], []))
    def place_druid_on_closest_visible_tile(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().cubic
        visible_tiles = sorted(visible_tiles, key=self.get_distance_from_king)
        for tile in visible_tiles:
            if not self.map_proxy.is_position_occupied(tile) and tile not in base_position.get_all_neighbours():
                # try: neco except errorType neco except neco
                self.game_control_proxy.spawn_unit(
                    SpawnInformation(self.player,
                                     GameObjectType.DRUID,
                                     tile,
                                     [], []))
                break

    def place_archer_on_all_visible_villages(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().cubic
        for tile in visible_tiles:
            if not self.map_proxy.is_position_occupied(tile) and self.map_proxy.get_terrain_type(tile) == TerrainType.VILLAGE and tile not in base_position.get_all_neighbours():
                self.game_control_proxy.spawn_unit(
                    SpawnInformation(self.player,
                                     GameObjectType.ARCHER,
                                     tile,
                                     [], []))

    # non action methods
    def get_distance_from_king(self, position):
        base_position = self.map_proxy.get_bases_positions().pop().cubic
        return position.cubic.distance_from(base_position)
