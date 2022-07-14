from typing import List
from OrodaelTurrim.Business.Interface.Player import PlayerTag
from OrodaelTurrim.Business.Proxy import MapProxy, GameObjectProxy, GameUncertaintyProxy
from ExpertSystem.Business.UserFramework import IKnowledgeBase
from ExpertSystem.Structure.RuleBase import Fact
from OrodaelTurrim.Structure.Enums import TerrainType, AttributeType, EffectType, GameRole, GameObjectType, \
    HexDirection
from OrodaelTurrim.Structure.Position import OffsetPosition
import sys


class KnowledgeBase(IKnowledgeBase):
    """
    Class for defining known facts based on Proxy information. You can transform here any information from
    proxy to better format of Facts. Important is method `create_knowledge_base()`. Return value of this method
    will be passed to `Inference.interfere`. It is recommended to use Fact class but you can use another type.

    | Class provides attributes:

    - **map_proxy [MapProxy]** - Proxy for access to map information
    - **game_object_proxy [GameObjectProxy]** - Proxy for access to all game object information
    - **uncertainty_proxy [UncertaintyProxy]** - Proxy for access to all uncertainty information in game
    - **player [PlayerTag]** - class that serve as instance of user player for identification in proxy methods

    """
    map_proxy: MapProxy
    game_object_proxy: GameObjectProxy
    game_uncertainty_proxy: GameUncertaintyProxy
    player: PlayerTag

    def __init__(self, map_proxy: MapProxy, game_object_proxy: GameObjectProxy,
                 game_uncertainty_proxy: GameUncertaintyProxy, player: PlayerTag):
        """
        You can add some code to __init__ function, but don't change the signature. You cannot initialize
        KnowledgeBase class manually so, it is make no sense to change signature.
        """
        super().__init__(map_proxy, game_object_proxy, game_uncertainty_proxy, player)

    def create_knowledge_base(self) -> List[Fact]:
        """
        Method for create user knowledge base. You can also have other class methods, but entry point must be this
        function. Don't change the signature of the method, you can change return value, but it is not recommended.
        """
        facts = []

        # Add bool fact
        if not self.map_proxy.player_have_base(self.player):
            facts.append(Fact('player_dont_have_base'))

        # fact indicating that it's odd round
        facts.append(Fact('odd_round', lambda: self.game_object_proxy.get_current_round() % 2 != 0))

        facts.append(Fact('find_suitable_place_for_king', eval_function=self.find_suitable_place_for_king,
                          data=self.find_suitable_place_for_king))

        facts.append(Fact('visible_free_tile', eval_function=self.visible_free_tile, data=self.visible_free_tile))

        # Add numerical fact
        facts.append(Fact('money', lambda: self.game_object_proxy.get_resources(self.player)))
        # print(f'Mam {self.game_object_proxy.get_resources(self.player)} penez')

        facts.append(Fact('position_is_free', eval_function=self.is_position_free, data=self.is_position_free))
        facts.append(Fact('free_tile_near_king', eval_function=self.find_free_tile_near_king, data=self.find_free_tile_near_king))

        # base_is_unprotected from given direction
        facts.append(Fact('direction_from_base_is_empty', eval_function=self.check_base_surroundings,
                            data=self.check_base_surroundings))

        if self.map_proxy.player_have_base(self.player):
            base_position = self.map_proxy.get_bases_positions().pop().cubic

            # check if base is protected from all sites
            is_protected = True
            for neighbour in base_position.get_all_neighbours():
                if self.game_object_proxy.get_object_type(neighbour) not in GameObjectType.defenders():
                    is_protected = False
                    break
            if is_protected:
                facts.append(Fact('base_protected'))

            # check if there is enemy next to base
            for neighbour in base_position.get_all_neighbours():
                if self.game_object_proxy.get_object_type(neighbour) in GameObjectType.attackers():
                    facts.append(Fact('base_in_danger', eval_function=self.find_free_tile_near_king,
                                      probability=self.game_object_proxy.get_current_round()/35, data=self.find_free_tile_near_king))
            # obtain spawn info for next round and create facts based on it
            for spawn_info in self.future_spawn_info():
                fact_name = '' + spawn_info.get('type') + '_approaching_from_' + spawn_info.get('dir')
                pst = float(spawn_info.get('pst'))
                # print('factName: ' + fact_name + ' pst: ' + str(pst))
                # placing defending troop as close to base as possible in correct direction
                facts.append(Fact(fact_name, probability=pst, eval_function=self.free_tile_in_direction,
                                  data=self.free_tile_in_direction))

            facts.append(Fact('base_low_on_health',
                              probability=(self.game_object_proxy.get_current_hit_points(self.map_proxy.get_bases_positions().pop()) - 500) / (-500),
                              eval_function=self.find_free_tile_near_king, data=self.find_free_tile_near_king))

        return facts

    def find_suitable_place_for_king(self):
        middle = OffsetPosition(0, 0)
        border_tiles = self.map_proxy.get_border_tiles()
        positions_to_check = []
        if self.map_proxy.get_terrain_type(middle) in [TerrainType.HILL, TerrainType.FOREST, TerrainType.VILLAGE]:
            return middle
        else:
            positions_to_check += middle.get_all_neighbours()
        counter = 0
        fields = []
        while positions_to_check[0] not in border_tiles:
            position = positions_to_check.pop(0)
            counter += 1
            if counter == 48 and len(fields) != 0:
                return fields[0]
            if self.map_proxy.get_terrain_type(position) in [TerrainType.HILL, TerrainType.FOREST, TerrainType.VILLAGE]:
                return position
            elif self.map_proxy.get_terrain_type(position) == TerrainType.FIELD:
                fields.append(position)
            else:
                positions_to_check += position.get_all_neighbours()
        return None

    def visible_free_tile(self, terrain_type: str):
        """ Find random free tile with given terrain type """
        tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()

        for position in tiles:
            terrain = self.map_proxy.get_terrain_type(position) == TerrainType.from_string(terrain_type)
            occupied = self.map_proxy.is_position_occupied(position)
            if terrain and not occupied and position not in border_tiles:
                return position
        return None

    def possible_spawn_tiles(self):
        """ Get list of possible tiles, where enemy spawn a unit """
        spawn_info = self.game_uncertainty_proxy.spawn_information()

        next_round = spawn_info[0]

        possible_tiles = set()
        for unit in next_round:
            possible_tiles.update([x.position for x in unit.positions])

        return possible_tiles

    def is_position_free(self, position_q: int, position_r: int):
        offset_position = OffsetPosition(position_q, position_r)
        if offset_position not in self.map_proxy.get_border_tiles() \
                and self.game_object_proxy.get_object_type(offset_position) == GameObjectType.NONE:
            return offset_position
        return None

    def neighbour(self, direction: str):
        if len(self.map_proxy.get_bases_positions()) == 0:
            base_position = OffsetPosition(0, 0).cubic
        else:
            base_position = self.map_proxy.get_bases_positions().pop().cubic
        switcher = {
            'RIGHT_UP': base_position.neighbour(HexDirection.RIGHT_UPPER),
            'RIGHT_DOWN': base_position.neighbour(HexDirection.RIGHT_LOWER),
            'LEFT_UP': base_position.neighbour(HexDirection.LEFT_UPPER),
            'LEFT_DOWN': base_position.neighbour(HexDirection.LEFT_LOWER),
            'UP': base_position.neighbour(HexDirection.UPPER),
            'DOWN': base_position.neighbour(HexDirection.LOWER)
        }
        return switcher.get(direction, None)

    def check_base_surroundings(self, direction: str):
        position = self.neighbour(direction)
        if self.game_object_proxy.get_object_type(position) == GameObjectType.NONE:
            return position
        else:
            return None

    def enemy_type(self, type: GameObjectType):
        switcher = {
            GameObjectType.CYCLOPS: 'cyclop',
            GameObjectType.DEMON: 'demon',
            GameObjectType.ELEMENTAL: 'elemental',
            GameObjectType.GARGOYLE: 'gargoyle',
            GameObjectType.MINOTAUR: 'minotaur',
            GameObjectType.NECROMANCER: 'necromancer',
            GameObjectType.ORC: 'orc',
            GameObjectType.SKELETON: 'skeleton'
        }
        return switcher.get(type, None)

    def determine_dir_from_base(self, position):
        base_position = self.map_proxy.get_bases_positions().pop().offset
        if base_position.q - 2 > position.q:
            return 'left'
        elif base_position.q + 2 < position.q:
            return 'right'
        elif base_position.r > position.r:
            return 'down'
        else:
            return 'up'

    def enemy_threat_indicator(self, threat):
        switcher = {
            GameObjectType.CYCLOPS: 0.85,
            GameObjectType.DEMON: 1,
            GameObjectType.ELEMENTAL: 0.75,
            GameObjectType.GARGOYLE: 0.65,
            GameObjectType.MINOTAUR: 1,
            GameObjectType.NECROMANCER: 0.8,
            GameObjectType.ORC: 0.39,
            GameObjectType.SKELETON: 0.3
        }
        return switcher.get(threat, None)

    def future_spawn_info(self):
        direction_to_cnt_dict = {
            'up': 0,
            'down': 0,
            'right': 0,
            'left': 0
        }
        base_position = self.map_proxy.get_bases_positions().pop().offset
        spawn_info = []
        threats = []
        most_dangerous_threat = None
        enemy_threat_sum = 0
        for uncertain_spawn in self.game_uncertainty_proxy.spawn_information()[0]:
            closest_position_distance = sys.float_info.max
            closest_position = None
            threats.append(uncertain_spawn.game_object_type)
            for spawn_position in uncertain_spawn.positions:
                if base_position.distance_from(spawn_position.position) < closest_position_distance:
                    closest_position_distance = base_position.distance_from(spawn_position.position)
                    closest_position = spawn_position.position
            if most_dangerous_threat is None\
                    or self.enemy_threat_indicator(most_dangerous_threat) < self.enemy_threat_indicator(uncertain_spawn.game_object_type):
                most_dangerous_threat = uncertain_spawn.game_object_type
            direction_to_cnt_dict[self.determine_dir_from_base(closest_position.offset)] += 1*self.enemy_threat_indicator(uncertain_spawn.game_object_type)
            enemy_threat_sum += self.enemy_threat_indicator(uncertain_spawn.game_object_type)

        most_occurred_direction = None
        for key in direction_to_cnt_dict.keys():
            if most_occurred_direction is None\
                    or direction_to_cnt_dict[most_occurred_direction] < direction_to_cnt_dict[key]:
                most_occurred_direction = key

        pst = round(direction_to_cnt_dict[most_occurred_direction] / enemy_threat_sum, 3)

        spawn_info.append(
            {
                'type': self.enemy_type(most_dangerous_threat),
                'dir': most_occurred_direction,
                'pst': pst
            }
        )
        return spawn_info

    # find closest free tile from base in given direction
    def free_tile_in_direction(self, direction: str):
        if direction == 'right':
            return_value = self.free_tile_in_right_direction()
            if return_value is None:
                return_value = self.free_tile_in_down_direction()
            if return_value is None:
                return_value = self.free_tile_in_up_direction()
            if return_value is None:
                return_value = self.free_tile_in_left_direction()
        elif direction == 'left':
            return_value = self.free_tile_in_left_direction()
            if return_value is None:
                return_value = self.free_tile_in_up_direction()
            if return_value is None:
                return_value = self.free_tile_in_down_direction()
            if return_value is None:
                return_value = self.free_tile_in_right_direction()
        elif direction == 'up':
            return_value = self.free_tile_in_up_direction()
            if return_value is None:
                return_value = self.free_tile_in_left_direction()
            if return_value is None:
                return_value = self.free_tile_in_right_direction()
            if return_value is None:
                return_value = self.free_tile_in_down_direction()
        else:
            return_value = self.free_tile_in_down_direction()
            if return_value is None:
                return_value = self.free_tile_in_left_direction()
            if return_value is None:
                return_value = self.free_tile_in_right_direction()
            if return_value is None:
                return_value = self.free_tile_in_up_direction()

    def free_tile_in_right_direction(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().offset
        posible_tile_for_placement = None
        for visible_tile in visible_tiles:
            if base_position.q < visible_tile.offset.q and visible_tile not in border_tiles \
                    and not self.map_proxy.is_position_occupied(visible_tile):
                if posible_tile_for_placement is None \
                        or base_position.distance_from(visible_tile) < base_position.distance_from(posible_tile_for_placement):
                    posible_tile_for_placement = visible_tile
        return posible_tile_for_placement

    def free_tile_in_left_direction(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().offset
        posible_tile_for_placement = None
        for visible_tile in visible_tiles:
            if base_position.q > visible_tile.offset.q and visible_tile not in border_tiles \
                    and not self.map_proxy.is_position_occupied(visible_tile):
                if posible_tile_for_placement is None \
                        or base_position.distance_from(visible_tile) < base_position.distance_from(
                    posible_tile_for_placement):
                    posible_tile_for_placement = visible_tile
        return posible_tile_for_placement

    def free_tile_in_up_direction(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().offset
        posible_tile_for_placement = None
        for visible_tile in visible_tiles:
            if base_position.q - 2 <= visible_tile.offset.q <= base_position.q + 2 \
                    and base_position.r < visible_tile.offset.r and visible_tile not in border_tiles \
                    and not self.map_proxy.is_position_occupied(visible_tile):
                if posible_tile_for_placement is None \
                        or base_position.distance_from(visible_tile) < base_position.distance_from(
                    posible_tile_for_placement):
                    posible_tile_for_placement = visible_tile
        return posible_tile_for_placement

    def free_tile_in_down_direction(self):
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        border_tiles = self.map_proxy.get_border_tiles()
        base_position = self.map_proxy.get_bases_positions().pop().offset
        posible_tile_for_placement = None
        for visible_tile in visible_tiles:
            if base_position.q - 2 <= visible_tile.offset.q <= base_position.q + 2 \
                    and base_position.r > visible_tile.offset.r and visible_tile not in border_tiles \
                    and not self.map_proxy.is_position_occupied(visible_tile):
                if posible_tile_for_placement is None \
                        or base_position.distance_from(visible_tile) < base_position.distance_from(
                    posible_tile_for_placement):
                    posible_tile_for_placement = visible_tile
        return posible_tile_for_placement

    def find_free_tile_near_king(self):
        border_tiles = self.map_proxy.get_border_tiles()
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        base_position = self.map_proxy.get_bases_positions().pop()
        positions_to_check = base_position.get_all_neighbours()
        hills = []
        while positions_to_check[0] not in border_tiles and positions_to_check[0] in visible_tiles:
            position = positions_to_check.pop(0)
            if not self.map_proxy.is_position_occupied(position):
                if self.map_proxy.get_terrain_type(position) == TerrainType.HILL:
                    hills.append(position)
                else:
                    return position
            positions_to_check += position.get_all_neighbours()

        if len(hills) != 0:
            return hills[0]

        return None
