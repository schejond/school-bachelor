(define (problem map-3)
    (:domain fisherman)
    (:requirements :strips :typing)
    (:objects Les Reka Hospoda Pristav Mesto Akademie Majak More Ostrov - place
    )
    (:init
        (neighbour Les Reka)
        (neighbour Reka Les)
        (neighbour Reka Pristav)
        (neighbour Pristav Reka)
        (neighbour Hospoda Pristav)
        (neighbour Pristav Hospoda)
        (neighbour Pristav Mesto)
        (neighbour Mesto Pristav)
        (neighbour Mesto Akademie)
        (neighbour Akademie Mesto)
        (neighbour Pristav Majak)
        (neighbour Majak Pristav)
        (neighbour Pristav More)
        (neighbour More Pristav)
        (neighbour More Ostrov)
        (neighbour Ostrov More)

        (hasFisherman Pristav)
        
        (canWalk Les)
        (canWalk Reka)
        (canWalk Pristav)
        (canWalk Hospoda)
        (canWalk Mesto)
        (canWalk Akademie)

        (needsBoat Majak)
        (needsBoat More)
        (needsBoat Ostrov)

        (isSober)

        ; (at Les flower)

        (isForest Les)
        (isRiver Reka)
        (isPort Pristav)
        (isPub Hospoda)
        (isCity Mesto)
        (isAcademy Akademie)
        (isSea More)
        (isLightHouse Majak)
        (isIsland Ostrov)

        (piratesActive)

        (hasNoCriminalRecord)
        (cantFight)
    )

    (:goal 	(and
    			(hasHappyLife)
    		)
    )

)