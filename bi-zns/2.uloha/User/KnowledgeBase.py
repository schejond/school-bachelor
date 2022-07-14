from typing import List, Set

from OrodaelTurrim.Business.Interface.Player import IPlayer
from OrodaelTurrim.Business.Proxy import MapProxy, GameObjectProxy, GameUncertaintyProxy
from ExpertSystem.Business.UserFramework import IKnowledgeBase
from ExpertSystem.Structure.RuleBase import Fact
from OrodaelTurrim.Structure.Enums import GameObjectType, HexDirection, TerrainType


class KnowledgeBase(IKnowledgeBase):
    """
    Class for defining known facts based on Proxy information. You can transform here any information from
    proxy to better format of Facts. Important is method `create_knowledge_base()`. Return value of this method
    will be passed to `Interference.interfere`. It is recommended to use Fact class but you can use another type.

    |
    |
    | Class provides attributes:

    - **map_proxy [MapProxy]** - Proxy for access to map information
    - **game_object_proxy [GameObjectProxy]** - Proxy for access to all game object information
    - **uncertainty_proxy [UncertaintyProxy]** - Proxy for access to all uncertainty information in game
    - **player [IPlayer]** - instance of user player for identification in proxy methods

    """
    map_proxy: MapProxy
    game_object_proxy: GameObjectProxy
    game_uncertainty_proxy: GameUncertaintyProxy
    player: IPlayer


    def __init__(self, map_proxy: MapProxy, game_object_proxy: GameObjectProxy,
                 game_uncertainty_proxy: GameUncertaintyProxy, player: IPlayer):
        """
        You can add some code to __init__ function, but don't change the signature. You cannot initialize
        KnowledgeBase class manually so, it is make no sense to change signature.
        """
        super().__init__(map_proxy, game_object_proxy, game_uncertainty_proxy, player)


    def create_knowledge_base(self) -> List[Fact]:
        """
        Method for create user knowledge base. You can also have other class methods, but entry point must be this
        function. Don't change the signature of the method, you can change return value, but it is not recommended.

        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!  TODO: Write implementation of your knowledge base definition HERE   !!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        """

        facts = []

        # Add bool fact
        if not self.map_proxy.player_have_base(self.player):
            facts.append(Fact('player_dont_have_base'))

        # Add numerical fact
        user_resources = self.game_object_proxy.get_resources(self.player)
        facts.append(Fact("money", lambda: user_resources))

        if self.map_proxy.player_have_base(self.player):
            base_position = self.map_proxy.get_bases_positions().pop().cubic

            # base_is_unprotected__from_right_up
            base_right_up_neighbour = base_position.neighbour(HexDirection.RIGHT_UPPER)
            if self.game_object_proxy.get_object_type(base_right_up_neighbour) == GameObjectType.NONE:
                facts.append(Fact('right_up_from_base_is_empty'))

            # base_is_unprotected__from_right_down
            base_right_down_neighbour = base_position.neighbour(HexDirection.RIGHT_LOWER)
            if self.game_object_proxy.get_object_type(base_right_down_neighbour) == GameObjectType.NONE:
                facts.append(Fact('right_down_from_base_is_empty'))

            # base_is_unprotected__from_left_up
            base_left_up_neighbour = base_position.neighbour(HexDirection.LEFT_UPPER)
            if self.game_object_proxy.get_object_type(base_left_up_neighbour) == GameObjectType.NONE:
                facts.append(Fact('left_up_from_base_is_empty'))

            # base_is_unprotected__from_left_down
            base_left_down_neighbour = base_position.neighbour(HexDirection.LEFT_LOWER)
            if self.game_object_proxy.get_object_type(base_left_down_neighbour) == GameObjectType.NONE:
                facts.append(Fact('left_down_from_base_is_empty'))

            # base_is_unprotected__from_up
            base_up_neighbour = base_position.neighbour(HexDirection.UPPER)
            if self.game_object_proxy.get_object_type(base_up_neighbour) == GameObjectType.NONE:
                facts.append(Fact('up_from_base_is_empty'))

            # base_is_unprotected__from_down
            base_down_neighbour = base_position.neighbour(HexDirection.LOWER)
            if self.game_object_proxy.get_object_type(base_down_neighbour) == GameObjectType.NONE:
                facts.append(Fact('down_from_base_is_empty'))

            # if player have enough resources for knight
            if self.game_object_proxy.get_resources(self.player) >= GameObjectType.KNIGHT.price:
                facts.append(Fact('has_enough_resources_for_knight'))

            # if player have enough resources for magician
            if self.game_object_proxy.get_resources(self.player) >= GameObjectType.MAGICIAN.price:
                facts.append(Fact('has_enough_resources_for_magician'))

            # if player have enough resources for archer
            if self.game_object_proxy.get_resources(self.player) >= GameObjectType.ARCHER.price:
                facts.append(Fact('has_enough_resources_for_archer'))

            # if player has at least 130 gold
            if self.game_object_proxy.get_resources(self.player) >= 130:
                facts.append(Fact('has_at_least_130_gold'))

            visible_tiles = self.map_proxy.get_player_visible_tiles()
            visible_tiles = sorted(visible_tiles, key=self.get_distance_from_king)
            for tile in visible_tiles:
                if self.game_object_proxy.get_object_type(tile) == GameObjectType.NONE:
                    facts.append(Fact('some_visible_tile_is_not_occupied'))
                if self.map_proxy.get_terrain_type(tile) == TerrainType.VILLAGE and self.game_object_proxy.get_object_type(tile) == GameObjectType.NONE:
                    facts.append(Fact('empty_village_is_visible'))

        return facts

    def get_distance_from_king(self, position):
        base_position = self.map_proxy.get_bases_positions().pop().cubic
        return position.cubic.distance_from(base_position)
