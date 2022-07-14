#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;
//---------------------------------------------------------------------------------------------------
class City {
private:
    long long int myIndex;
    long long int myGrocery;
    long long int minimalCntOfGroceries;
    long long int sumOfDistances;

    vector<City*> neighbourCitiesVector;
    vector<pair<long long int, long long int>> pairGroceryWithDistanceVector;
    long long int distances[100];
//---------------------------------------------------------------------------------------------------  
public:
    City()
    :   myGrocery(-1), minimalCntOfGroceries(1), sumOfDistances(0) {
        for (long long int i = 0 ; i < 100 ; i++) {
            distances[i] = -1;
        }
    }

    ~City() = default;

    long long int const getMyGrocery() {
        return myGrocery;
    }

    long long int getMyIndex() {
        return myIndex;
    }

    void setMyGrocery(const long long int myGrocery, const long long int myIndex) {
        this->myGrocery = myGrocery;
        this->myIndex = myIndex;

        distances[myGrocery] = 0;
        pairGroceryWithDistanceVector.push_back(make_pair(myGrocery, 0));
    }

    void setMinimalCntOfGroceries(const long long int minimalCntOfGroceries) {
        this->minimalCntOfGroceries = minimalCntOfGroceries;
    }

    vector<City*> & getNeighbourCities() {
        return neighbourCitiesVector;
    }

    void addNeighbour(City *neighbour) {
        this->neighbourCitiesVector.push_back(neighbour);
    }

    void checkGroceriesPresentAndInsertDistance(const long long int grocery, const long long int distance) {
        if (distances[grocery] == -1) {
            distances[grocery] = distance;
        }
    }

    long long int sortAndCountDistance() {
        for (long long int i = 0 ; i < 100 ; i++) {
            if (distances[i] != -1 && distances[i] != 0) {
                pairGroceryWithDistanceVector.push_back(make_pair(i, distances[i]));
            }
        }
    	//sort by the second value in pair
    	sort(pairGroceryWithDistanceVector.begin(), pairGroceryWithDistanceVector.end(),
            [](auto &left, auto &right) {return left.second < right.second;}
        );
        //collect specific number (minimalCntOfGroceries) of groceries and count their distances and put them into finalGroceriesSet
		for (long long int i = 0 ; i < minimalCntOfGroceries ; i++) {
			sumOfDistances += pairGroceryWithDistanceVector[i].second;
		}
		return sumOfDistances;
    }

    void const print() {
        cout << sumOfDistances << " ";
        
        for (long long int i = 0 ; i < minimalCntOfGroceries ; i++) {
            cout << pairGroceryWithDistanceVector[i].first;
            if (i < minimalCntOfGroceries - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
};
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
class CountryMap {
private:
    const long long int cityCnt;
    const long long int routeCnt;
    const long long int groceriesCnt;
    const long long int minimalCntOfGroceriesInCity;

    long long int finalRoutesCnt;

    map<long long int, vector<City*>> groceryToVectorOfCityPtrMap;
    vector<City> cityCollection;
public:
    CountryMap(const long long int cityCnt, const long long int routeCnt, const long long int groceriesCnt, const long long int minimalCntOfGroceriesInCity)
            : cityCnt(cityCnt), routeCnt(routeCnt), groceriesCnt(groceriesCnt), minimalCntOfGroceriesInCity(minimalCntOfGroceriesInCity), finalRoutesCnt(0) {
                cityCollection.reserve(cityCnt);
    }

    ~CountryMap() = default;

    long long int const getCityCnt() {
        return cityCnt;
    }

    long long int const getRouteCnt() {
        return routeCnt;
    }
    //also sets the minimalCntOfGroceriesInCity for the city
    //and add item to groceryToVectorOfCityPtrMap map
    void addGroceryToCity(const long long int indexOfCity, const long long int indexOfGrocery) {
        cityCollection.emplace_back();
        City *targetCity = &cityCollection[indexOfCity];
        targetCity->setMyGrocery(indexOfGrocery, indexOfCity);
        targetCity->setMinimalCntOfGroceries(minimalCntOfGroceriesInCity);

		groceryToVectorOfCityPtrMap[indexOfGrocery].push_back(targetCity);
    }

    void addRoute(const long long int cityIndex1, const long long int cityIndex2) {
        cityCollection[cityIndex1].addNeighbour(&cityCollection[cityIndex2]);
        cityCollection[cityIndex2].addNeighbour(&cityCollection[cityIndex1]);
    }

    void bfs(const long long int startingGrocery, queue<pair<City*, long long int>> & queuePairCityPtrWithDistance, vector<bool> & visitedCities) {
    	if (queuePairCityPtrWithDistance.empty()) {
    		return;
    	}

    	const long long int distance = queuePairCityPtrWithDistance.front().second + 1;
    	City *currentCity = queuePairCityPtrWithDistance.front().first;

    	for (City * neighbourCity : currentCity->getNeighbourCities()) {
    		
            neighbourCity->checkGroceriesPresentAndInsertDistance(startingGrocery, distance);

    		if (!visitedCities[neighbourCity->getMyIndex()]) {
                queuePairCityPtrWithDistance.push(make_pair(neighbourCity,distance));
                visitedCities[neighbourCity->getMyIndex()] = true;
            }
    	}
    	queuePairCityPtrWithDistance.pop();
    	bfs(startingGrocery, queuePairCityPtrWithDistance, visitedCities);
    }

    void collectAllData() {
    	vector<bool> visitedCities;
        for (long long int i = 0 ; i < cityCnt ; i++) {
            visitedCities.push_back(false);
        }
        for (map<long long int, vector<City*>>::iterator mapIterator = groceryToVectorOfCityPtrMap.begin() ; mapIterator != groceryToVectorOfCityPtrMap.end() ; mapIterator++) {
    		for (long long int i = 0 ; i < cityCnt ; i++) {
                visitedCities[i] = false;
            }
            queue<pair<City*, long long int>> queuePairCityPtrWithDistance;
    		for (City* cityPtrItem : mapIterator->second) {
    			queuePairCityPtrWithDistance.push(make_pair(cityPtrItem, 0));
                visitedCities[cityPtrItem->getMyIndex()] = true;
    		}
    		bfs(mapIterator->first, queuePairCityPtrWithDistance, visitedCities);
    	}
    	calculateFinalRoutesCnt();
    }

    void calculateFinalRoutesCnt() {
    	for (unsigned long long int i = 0 ; i < cityCollection.size() ; i++) {
            finalRoutesCnt += cityCollection[i].sortAndCountDistance();
        }
    }

    void const print() {
        cout << finalRoutesCnt << endl;
        for (unsigned long long int i = 0 ; i < cityCollection.size() ; i++) {
            cityCollection[i].print();
        }
    }
};
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
int main() {
    long long int cityCnt = -1; //N
    long long int routesCnt = -1; //M
    long long int groceceriesTypeCnt = -1; //P
    long long int minimalVarietyOfGroceriesInOneCity = -1; //Q

    cin >> cityCnt >> routesCnt;
    cin >> groceceriesTypeCnt >> minimalVarietyOfGroceriesInOneCity;

    CountryMap cities(cityCnt, routesCnt, groceceriesTypeCnt, minimalVarietyOfGroceriesInOneCity);

    for (long long int cityIndex = 0 ; cityIndex < cities.getCityCnt() ; cityIndex++) {
        long long int groceryIndex;
        cin >> groceryIndex;
        cities.addGroceryToCity(cityIndex, groceryIndex);
    }

    for (long long int i = 0 ; i < cities.getRouteCnt() ; i++) {
        long long int cityIndex1, cityIndex2;
        cin >> cityIndex1 >> cityIndex2;
        cities.addRoute(cityIndex1, cityIndex2);
    }

    if (minimalVarietyOfGroceriesInOneCity == 1) {
        cities.print();
        return 0;
    }

    cities.collectAllData();
    cities.print();

    return 0;
}