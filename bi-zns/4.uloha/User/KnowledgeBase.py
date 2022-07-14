from typing import List
from OrodaelTurrim.Business.Interface.Player import PlayerTag
from OrodaelTurrim.Business.Proxy import MapProxy, GameObjectProxy, GameUncertaintyProxy
from ExpertSystem.Business.UserFramework import IKnowledgeBase
from ExpertSystem.Structure.RuleBase import Fact
from OrodaelTurrim.Structure.Enums import TerrainType, AttributeType, EffectType, GameRole, GameObjectType, \
    HexDirection
from OrodaelTurrim.Structure.Position import OffsetPosition


class KnowledgeBase(IKnowledgeBase):
    """
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
        facts = []

        if not self.map_proxy.player_have_base(self.player):
            facts.append(Fact('player_dont_have_base'))

        facts.append(Fact('find_suitable_place_for_king', eval_function=self.find_suitable_place_for_king,
                          data=self.find_suitable_place_for_king))

        facts.append(Fact('visible_free_tile', eval_function=self.visible_free_tile, data=self.visible_free_tile))

        facts.append(Fact('money', lambda: self.game_object_proxy.get_resources(self.player)))
        # print(f'Mam {self.game_object_proxy.get_resources(self.player)} penez')

        facts.append(Fact('position_is_free', eval_function=self.is_position_free, data=self.is_position_free))
        facts.append(Fact('free_tile_near_king', eval_function=self.find_free_tile_near_king, data=self.find_free_tile_near_king))

        # base_is_unprotected from given direction
        facts.append(Fact('direction_from_base_is_empty', eval_function=self.check_base_surroundings,
                            data=self.check_base_surroundings))

        if self.map_proxy.player_have_base(self.player):
            base_position = self.map_proxy.get_bases_positions().pop().cubic

            facts.append(Fact('base_health', lambda: self.game_object_proxy.get_current_hit_points(base_position)))

            facts.append(Fact('closest_enemy_distance', lambda: self.get_distance_to_closest_visible_enemy))

            # check if base is protected from all sites
            is_protected = True
            for neighbour in base_position.get_all_neighbours():
                if self.game_object_proxy.get_object_type(neighbour) not in GameObjectType.defenders():
                    is_protected = False
                    break
            if is_protected:
                facts.append(Fact('base_protected'))
        # else:
        #     facts.append(Fact('base_health', lambda: 0))
        #     facts.append(Fact('closest_enemy_distance', lambda: 0))
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

    def find_free_tile_near_king(self):
        enemy_next_to_base = False
        base_position = self.map_proxy.get_bases_positions().pop()
        for neighbour in base_position.get_all_neighbours():
            if self.game_object_proxy.get_object_type(neighbour) in GameObjectType.attackers():
                enemy_next_to_base = True
                break
        if self.game_object_proxy.get_current_hit_points(base_position) >= 350 and not enemy_next_to_base:
            return None

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

    def get_distance_to_closest_visible_enemy(self):
        base_position = self.map_proxy.get_bases_positions().pop()
        visible_tiles = self.map_proxy.get_player_visible_tiles()
        visible_enemies = []
        for tile in visible_tiles:
            if self.game_object_proxy.get_object_type(tile) in GameObjectType.attackers():
                visible_enemies.append(tile)

        if len(visible_enemies) == 0:
            return 9

        minimum_distance = 21
        for tile in visible_enemies:
            if base_position.distance_from(tile) < minimum_distance:
                minimum_distance = base_position.distance_from(tile)

        print('vracim', minimum_distance, type(minimum_distance))
        return minimum_distance
