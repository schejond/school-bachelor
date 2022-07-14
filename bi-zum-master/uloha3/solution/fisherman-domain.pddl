(define (domain fisherman)
    (:requirements :strips :typing)
    (:types place collectible)
    (:predicates
		(neighbour ?place1 ?place2)
		(hasFisherman ?place)
		(canWalk ?place) ;tzn muzu tam jit pesky
		(needsBoat ?place)
		(canSail)
		(cantFight)

		;defines places
		(isForest ?place)
		(isRiver ?place)
		(isPort ?place)
		(isPub ?place)
		(isCity ?place)
		(isAcademy ?place)
		(isSea ?place)
        (isLightHouse ?place)
    	(isIsland ?place)

		;forest specific predicates
		(hasWood)
		(hasBearSkin)
		(hasMap)
		(hasStrangeConnection)
		(hasFlower)
		(hasFightingSkills)
    	(fightedBear)

		;river specific
		;none

		;port specific
		(hasGoldGrain)
		(hasGoldCoin)
		(hasGoldBrick)
		(knowsSmugglers)

		;pub specific
   		(hasGoodConnections)

   		;city specific
   		;none

   		;sea academy specific
    	(isCaptain)

    	;sea specific
    	(piratesActive)
    	(hasPearl)
    	(seaSafe)
		(defeatedPirates)

    	;lighthouse specific
    	(isMarried)

    	;island specific
    	(hasCoconut)
    	(hasCocaine)

    	(isAddictedOnCocaine)

		;fishermans drunk stages
		(isSober)
		(isLightlyDrunk)
		(isDrunk)
		(isAlcoholic)

		(hasCriminalRecord)
		(hasNoCriminalRecord)

		(hasBoat)
		(hasFrigate)
		(hasKaravel)

		(hasRing)

		(hasAlcohol)

		(hasHappyLife)
    )
;-----------------------------------------------------    
;move actions
    (:action landMove ;pesky po nozickach
        :parameters (?from - place ?to - place)
        :precondition (and
            (neighbour ?from ?to)
            (hasFisherman ?from)
            (canWalk ?to)
        )
        :effect (and
        	(not (hasFisherman ?from))
        	(hasFisherman ?to)
        )
    )

    (:action sailMove
        :parameters (?from - place ?to - place)
        :precondition (and
            (neighbour ?from ?to)
            (hasFisherman ?from)
            (needsBoat ?to)
            (canSail)
        )
        :effect (and
        	(not (hasFisherman ?from))
        	(hasFisherman ?to)
        )
    )
;////////////////////////////////////////////////////////////
;drinking actions
    (:action getLightlyDrunk
        :parameters ()
        :precondition (and
            (hasAlcohol)
            (isSober)
        )
        :effect (and
        	(not (isSober))
        	(not (hasAlcohol))
        	(isLightlyDrunk)
        )
    )
    (:action getDrunk
        :parameters ()
        :precondition (and
            (hasAlcohol)
            (isLightlyDrunk)
        )
        :effect (and
        	(not (isLightlyDrunk))
        	(not (hasAlcohol))
        	(isDrunk)
        )
    )
    (:action becomeAlcoholic
        :parameters ()
        :precondition (and
            (hasAlcohol)
            (isDrunk)
        )
        :effect (and
        	(not (isDrunk))
        	(not (hasAlcohol))
        	(isAlcoholic)
        )
    )    
;////////////////////////////////////////////////////////////
;building boat action
    (:action buildBoat
        :parameters ()
        :precondition (and
            (hasWood)
        )
        :effect (and
        	(hasBoat)
        	(not (hasWood))
        	(canSail)
        )
    )
    (:action buildFrigate
        :parameters ()
        :precondition (and
            (hasBoat)
            (hasWood)
            (hasGoldGrain)
        )
        :effect (and
        	(not (hasBoat))
        	(not (hasWood))
        	(not (hasGoldGrain))
        	(canSail)
        	(hasFrigate)
        )
    )
    (:action buildKaravel
        :parameters ()
        :precondition (and
        	(hasBoat)
            (hasWood)
            (hasGoldCoin)
        )
        :effect (and
        	(not (hasBoat))
        	(not (hasWood))
        	(not (hasGoldCoin))
        	(canSail)
            (hasKaravel)
        )
    )        
;////////////////////////////////////////////////////////////
;-----------------------------------------------------    
;les specific actions
    (:action lesPickUpFlower
    	:parameters (?place - place)
    	:precondition (and 
    		(isForest ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasFlower)
    	)
    )

    (:action lesPickUpWood
    	:parameters (?place - place)
    	:precondition (and 
    		(isForest ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasWood)
    	)
    )

    (:action lesFightBear
    	:parameters (?place - place)
    	:precondition (and 
    		(isForest ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(fightedBear)
    		(hasBearSkin)
    		(hasFightingSkills)
    		(not (cantFight))
    	)
    )

    (:action lesGetMap
    	:parameters (?place - place)
    	:precondition (and 
    		(isForest ?place)
    		(hasFisherman ?place)
    		(hasAlcohol)
    	)
    	:effect (and
    		(hasMap)
    		(not (hasAlcohol))
    		(hasStrangeConnection)
    	)
    )
;-----------------------------------------------------
;river actions
    (:action rekaStealBoat
    	:parameters (?place - place)
    	:precondition (and
    		(isRiver ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasCriminalRecord)
    		(not (hasNoCriminalRecord))
    		(hasBoat)
    	)
    )
    (:action rekaPanGold
    	:parameters (?place - place)
    	:precondition (and
    		(isRiver ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasGoldGrain)
    	)
    )
    ;todo je schopen vystrizlivet ze vsech 3 stavu opilosti?
    (:action rekaColdBathLightlyDrunk
    	:parameters (?place - place)
    	:precondition (and
    		(isRiver ?place)
    		(hasFisherman ?place)
    		(isLightlyDrunk)
    	)
    	:effect (and
    		(not (isLightlyDrunk))
    		(isSober)
    	)
    )
    (:action rekaColdBathDrunk
    	:parameters (?place - place)
    	:precondition (and
    		(isRiver ?place)
    		(hasFisherman ?place)
    		(isDrunk)
    	)
    	:effect (and
    		(not (isDrunk))
    		(isSober)
    	)
    )
    ; (:action rekaColdBathAlcoholic
    ; 	:parameters (?place - place)
    ; 	:precondition (and
    ; 		(isRiver ?place)
    ; 		(hasFisherman ?place)
    ; 		(isAlcoholic)
    ; 	)
    ; 	:effect (and
    ; 		(hasGoldGrain)
    ; 		(not (isAlcoholic))
    ; 		(isSober)
    ; 	)
    ; )        
;-----------------------------------------------------
;pristav actions
    (:action pristavWork
    	:parameters (?place - place)
    	:precondition (and 
    		(isPort ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasGoldGrain)
    	)
    )

    (:action pristavTradeBearSkin
    	:parameters (?place - place)
    	:precondition (and 
    		(isPort ?place)
    		(hasFisherman ?place)
    		(hasBearSkin)
    	)
    	:effect (and
    		(not (hasBearSkin))
			(hasGoldCoin)
    	)
    )   

    (:action pristavTradeCoconut
    	:parameters (?place - place)
    	:precondition (and 
    		(isPort ?place)
    		(hasFisherman ?place)
    		(hasCoconut)
    	)
    	:effect (and
    		(not (hasCoconut))
			(hasGoldCoin)
    	)
    ) 

    (:action pristavMeetSmugglers
    	:parameters (?place - place)
    	:precondition (and 
    		(isPort ?place)
    		(hasFisherman ?place)
    		(hasStrangeConnection)
    		(hasGoldBrick)
    	)
    	:effect (and
    		(knowsSmugglers)
    		;todo prijde o cihlu??
    		(not (hasGoldBrick))
    	)
    )          
;-----------------------------------------------------
;pub actions
    (:action hospodaBuyAlcohol
    	:parameters (?place - place)
    	:precondition (and 
    		(isPub ?place)
    		(hasFisherman ?place)
    		(hasGoldGrain)
    	)
    	:effect (and
    		(hasAlcohol)
    		(not (hasGoldGrain))
    	)
    )
    (:action hospodaDrinksOnMe
    	:parameters (?place - place)
    	:precondition (and 
    		(isPub ?place)
    		(hasFisherman ?place)
    		(hasGoldCoin)
    	)
    	:effect (and
    		(not (hasGoldCoin))
    		(hasGoodConnections)
    	)
    ) 
    (:action hospodaGetSoberInFight
    	:parameters (?place - place)
    	:precondition (and 
    		(isPub ?place)
    		(hasFisherman ?place)
    		(isLightlyDrunk)
    	)
    	:effect (and
    		(not (isLightlyDrunk))
    		(isSober)
    		(hasFightingSkills)
    		(not (cantFight))
    	)
    )
;-----------------------------------------------------
;city actions
    (:action mestoGoToBank
    	:parameters (?place - place)
    	:precondition (and 
    		(isCity ?place)
    		(hasFisherman ?place)
    		(hasGoldGrain)
    	)
    	:effect (and
    		(not (hasGoldGrain))
    		(hasGoldCoin)
    		(hasGoodConnections)
    	)
    )

    (:action mestoInvestInBank
    	:parameters (?place - place)
    	:precondition (and 
    		(isCity ?place)
    		(hasFisherman ?place)
    		(hasGoldCoin)
    	)
    	:effect (and
    		(not (hasGoldCoin))
    		(hasGoldBrick)
    		(hasGoodConnections)
    	)
    )

    (:action mestoStealCoin
    	:parameters (?place - place)
    	:precondition (and 
    		(isCity ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
    		(hasGoldCoin)
    		(hasCriminalRecord)
    		(not (hasNoCriminalRecord))
    	)
    )

    (:action mestoDeleteCriminalRecord
    	:parameters (?place - place)
    	:precondition (and 
    		(isCity ?place)
    		(hasFisherman ?place)
    		(hasCriminalRecord)
    		(hasGoldGrain)
    	)
    	:effect (and
    		(not (hasCriminalRecord))
    		(hasNoCriminalRecord)
    		(not (hasGoldGrain))
    	)
    )

    (:action mestoDeleteCriminalRecordByWorking
    	:parameters (?place - place)
    	:precondition (and 
    		(isCity ?place)
    		(hasFisherman ?place)
    		(hasCriminalRecord)
    		(isSober) ; todo ... jak to ma byt?
    	)
    	:effect (and
    		(hasGoldCoin)
    		(not (hasCriminalRecord))
    		(hasNoCriminalRecord)
    		(isLightlyDrunk)
    		(not (isSober))
    		; (not (isDrunk))
    		; (not (isAlcoholic))
    	)
    )
;-------------------------------------------------
;sea academy actions
    (:action akademieBecameCaptain
    	:parameters (?place - place)
    	:precondition (and 
    		(isAcademy ?place)
    		(hasFisherman ?place)
    		(hasNoCriminalRecord)
    		(hasGoldCoin)
    	)
    	:effect (and
    		(not (hasGoldCoin))
    		(isCaptain)
    	)
    )
;-------------------------------------------------
;sea actions
    (:action moreGetDefeatedByPirates
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(piratesActive)
    		(cantFight)
    	)
    	:effect (and
    		(hasFightingSkills)
    		(not (cantFight))
    		(not (hasGoldGrain))
    		(not (hasGoldCoin))
    		(not (hasGoldBrick))
    		(not (hasKaravel))
    		(not (hasFrigate))
    		(hasBoat)
    	)
    )
    (:action moreJoinPirates
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(hasStrangeConnection)
    		(piratesActive)
    	)
    	:effect (and
    		(not (isSober))
    		(isLightlyDrunk)
    		(not (piratesActive))
    		(seaSafe)
    	)
    )
    (:action moreDefeatPirates
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(hasKaravel)
    		(hasFightingSkills)
    		(piratesActive)
    	)
    	:effect (and
    		(hasBoat)
    		(hasFrigate)
    		(hasKaravel)
    		(hasGoldGrain)
    		(hasGoldCoin)
    		(hasGoldBrick)
    		(not (piratesActive))
    		(seaSafe)
    		(defeatedPirates)
    	)
    )
    (:action moreGetPearl
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(seaSafe)
    	)
    	:effect (and
    		(hasPearl)
    	)
    )
    (:action moreGetColdBathLightlyDrunk
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(seaSafe)
    		(isLightlyDrunk)
    	)
    	:effect (and
    		(not (isLightlyDrunk))
    		(isSober)
    	)
    )
    (:action moreGetColdBathDrunk
    	:parameters (?place - place)
    	:precondition (and 
    		(isSea ?place)
    		(hasFisherman ?place)
    		(seaSafe)
    		(isDrunk)
    	)
    	:effect (and
    		(not (isDrunk))
    		(isSober)
    	)
    )
;-------------------------------------------------
;lighthouse actions
    (:action majakGetMarriedBear
    	:parameters (?place - place)
    	:precondition (and 
    		(isLightHouse ?place)
    		(hasFisherman ?place)
    		(fightedBear)
    	)
    	:effect (and
    		(isMarried)
    	)
    )
    (:action majakGetMarriedCaptain
    	:parameters (?place - place)
    	:precondition (and 
    		(isLightHouse ?place)
    		(hasFisherman ?place)
    		(isCaptain)
    	)
    	:effect (and
    		(isMarried)
    	)
    )
    (:action majakGetMarriedPirates
    	:parameters (?place - place)
    	:precondition (and 
    		(isLightHouse ?place)
    		(hasFisherman ?place)
    		(defeatedPirates)
    	)
    	:effect (and
    		(isMarried)
    	)
    )
;-------------------------------------------------
;island actions
    (:action ostrovCollectCoconut
    	:parameters (?place - place)
    	:precondition (and 
    		(isIsland ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
			(hasCoconut)    		
    	)
    )
    (:action ostrovCollectWood
    	:parameters (?place - place)
    	:precondition (and 
    		(isIsland ?place)
    		(hasFisherman ?place)
    	)
    	:effect (and
			(hasWood)    		
    	)
    )
    (:action ostrovGetCocaine
    	:parameters (?place - place)
    	:precondition (and 
    		(isIsland ?place)
    		(hasFisherman ?place)
    		(hasMap)
    	)
    	:effect (and
    		(hasCocaine)
    	)
    )
;-------------------------------------------------
;other
    (:action buyRing
        :precondition (and
        	(hasGoldBrick)
        	(hasPearl)
        )
        :effect (and
        	(not (hasGoldBrick))
        	(not (hasPearl))
        	(hasRing)
        )
    )

    (:action becomeAddictedOnCocaine
        :precondition (and
        	(hasCocaine)
        )
        :effect (and
        	(isAddictedOnCocaine)
        )
    )

    (:action becomeHappyFromBecomingCaptain
        :precondition (and
        	(isCaptain)
        	(defeatedPirates)
        	(isSober)
        	(hasFisherman Akademie)
        )
        :effect (and
        	(hasHappyLife)
        )
    )


    (:action becomeHappyFromGettingMarried
        :precondition (and
    		(hasFlower)
        	(hasRing)
        	(isMarried)
        	(hasFisherman Ostrov)
        	(hasGoodConnections)
        	(hasNoCriminalRecord)
        	(isSober)
        )
        :effect (and
        	(hasHappyLife)
        )
    )

    (:action becomeHappyThanksToCocaine
        :precondition (and
    		(hasFrigate)
    		(knowsSmugglers)
    		(hasGoldBrick)
    		(isAddictedOnCocaine)
    		(hasCocaine)
        )
        :effect (and
        	(hasHappyLife)
        )
    )
)


