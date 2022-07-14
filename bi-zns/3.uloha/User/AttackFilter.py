from typing import List

from OrodaelTurrim.Business.Logger import Logger
from OrodaelTurrim.Business.Proxy import GameObjectProxy, MapProxy
from OrodaelTurrim.Structure.Enums import GameObjectType, AttributeType
from OrodaelTurrim.Structure.Filter.FilterPattern import AttackFilter
from OrodaelTurrim.Structure.Position import Position

""" 
In this file you can define your own attack filters if default filters are not enough for you. 
Filter must be subclass of class `AttackFilter` and must implement filter method with same signature

def filter(self, position: Position, tiles: List[Position]) -> List[Position]:
    pass

You can here define as many filters as you want. Framework will find correct filters and load them.
"""


class DummyAttackFilter(AttackFilter):
    """
    Example of custom filter with parameter.
    If you want to have a filter with a parameter, you must overload __init__ function. You must call parent __init__.

    Parameters map_proxy and game_object_proxy will be passed by FilterFactory.
    """

    def __init__(self, map_proxy: MapProxy, game_object_proxy: GameObjectProxy, log_text: str):
        super().__init__(map_proxy, game_object_proxy)
        self.log_text = log_text

    def filter(self, position: Position, tiles: List[Position]) -> List[Position]:
        """
        :param position: Current positions of game object
        :param tiles: List of all tiles, where unit could attack (based on visibility, attack range, etc.)
        :return: List of position, that left after filtering
        """
        Logger.log(self.log_text)
        return tiles


class ProtectBaseAttackFilter(AttackFilter):
    def filter(self, position: Position, tiles: List[Position]) -> List[Position]:
        filtered_tiles = []
        base_position = self.map_proxy.get_bases_positions().pop()
        for tile in tiles:
            if self.map_proxy.is_position_occupied(tile)\
                    and self.game_object_proxy.get_object_type(tile) in GameObjectType.attackers()\
                    and tile in base_position.get_all_neighbours():
                filtered_tiles.append(tile)
        return filtered_tiles


class TwoClosestToBasePositionsAttackFilter(AttackFilter):
    def filter(self, position: Position, tiles: List[Position]) -> List[Position]:
        filtered_tiles = []
        base_position = self.map_proxy.get_bases_positions().pop()
        for tile in tiles:
            if self.map_proxy.is_position_occupied(tile) \
                    and self.game_object_proxy.get_object_type(tile) in GameObjectType.attackers():
                filtered_tiles.append(tile)

        if len(filtered_tiles) > 1:
            filtered_tiles.sort(key=lambda tile: base_position.distance_from(tile))
            filtered_tiles = filtered_tiles[0:2]
            # pokud nejsou stejne daleko od basu, tak vrat jen tu blizsi
            if base_position.distance_from(filtered_tiles[0]) < base_position.distance_from(filtered_tiles[1]):
                return filtered_tiles[0:1]

        return filtered_tiles


class WhoICanDamageAttackFilter(AttackFilter):
    def filter(self, position: Position, tiles: List[Position]) -> List[Position]:
        filtered_tiles = []
        my_attack_power = self.game_object_proxy.get_attribute(position, AttributeType.ATTACK)
        for tile in tiles:
            if self.map_proxy.is_position_occupied(tile) \
                    and self.game_object_proxy.get_object_type(tile) in GameObjectType.attackers()\
                    and self.game_object_proxy.get_attribute(tile, AttributeType.DEFENSE) < my_attack_power:
                filtered_tiles.append(tile)
        return filtered_tiles
