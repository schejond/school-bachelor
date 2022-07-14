package logic;

import vwmschejond.Car;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;

public class Threshold {
    enum COLUMN {
        MAX_SPEED, PRICE, HORSE_POWER, AGE, DOOR_COUNT, TRUNK_VOLUME, SPEEDOMETER, TANK_VOLUME, OIL_VOLUME, NUMBER_OF_SEATS
    }

    enum AGGREGATION_FUNCTION {
        MAX, MIN, AVG
    }

    private List<CarWithAppliedAggregationFunction> topKObjects = new ArrayList<>();
    private int maxSpeedPosition = 0;
    private int pricePosition = 0;
    private int horsePowerPosition = 0;
    private int agePosition = 0;
    private int doorCountPosition = 0;
    private int trunkVolumePosition = 0;
    private int speedometerPosition = 0;
    private int tankVolumePosition = 0;
    private int oilVolumePosition = 0;
    private int numberOfSeatsPosition = 0;

    private float thresholdValue = 1;

    private AGGREGATION_FUNCTION selectedFunction = AGGREGATION_FUNCTION.MAX;

    private long timeSec;
    private long timeBehindDot;

    //variables for which field to search
    private boolean searchMaxSpeed = true;
    private boolean searchPrice = true;
    private boolean searchHorsePower = true;
    private boolean searchAge = true;
    private boolean searchDoorCount = true;
    private boolean searchTrunkVolume = true;
    private boolean searchSpeedometer = true;
    private boolean searchTankVolume = true;
    private boolean searchOilVolume = true;
    private boolean searchNumberOfSeats = true;

    private long seenObjects = 0;

    public Threshold() {
    }

    public void compute(OrderedColumns orderedColumns, final long k) {
        maxSpeedPosition = pricePosition = horsePowerPosition = agePosition = doorCountPosition = trunkVolumePosition =
                speedometerPosition = tankVolumePosition = oilVolumePosition = numberOfSeatsPosition = 0;
        final long timeBefore = System.nanoTime();

        while (true) {
            //read 1 object from parallel
            OrderedColumns.CarWithNormalizedValue car = getBestCarFromTopOfTheColumns(orderedColumns);
            CarWithAppliedAggregationFunction carForTopKList = new CarWithAppliedAggregationFunction();
            carForTopKList.setCar(car.getCar());
            carForTopKList.setValue(getValueForSelectedAggregateFunction(car, orderedColumns));

            if (!carForTopKList.getCar().isInTopList()) {
                topKObjects.add(carForTopKList);
                carForTopKList.getCar().setInTopList(true);
                seenObjects++;
            }

            //if k > number of cars in the database && we already have all objects from database
            //in the topKList => stop
            if (seenObjects == orderedColumns.getDoorCountColumn().size()) {
                break;
            }

            //if there is not enough elements in the list, we don't have to check the threshold yet
            if (topKObjects.size() < k) {
                continue;
            }
            //compute Threshold and check if there is enough elements with value > threshold
            computeThreshold(orderedColumns);
            topKObjects.sort(new AggregationValueComparator());
            //remove unnecessary objects
            while (topKObjects.size() > k) {
                topKObjects.remove((int)k);
            }
            int cnt = 0;
            int biggerValueItems = 0;
            for (CarWithAppliedAggregationFunction topCar : topKObjects) {
                if (topCar.getValue() >= thresholdValue) {
                    biggerValueItems++;
                }
                cnt++;
                if (cnt > k) {
                    break;
                }
            }
            if (biggerValueItems >= k) {
                break;
            }
        }

        topKObjects.sort(new AggregationValueComparator());
        //crop the topKList to contain just K objects
        List<CarWithAppliedAggregationFunction> justTopKObjects = new ArrayList<>();
        long i = 0;
        for (CarWithAppliedAggregationFunction car : topKObjects) {
            if (i > k) {
                break;
            }
            justTopKObjects.add(car);
            i++;
        }
        topKObjects.clear();
        topKObjects = justTopKObjects;

        //compute elapsed time
        final long timeAfter = System.nanoTime();
        final long timeNanoSec = timeAfter - timeBefore;
        timeSec = timeNanoSec/1000000000;
        timeBehindDot = timeNanoSec - timeSec*1000000000;
        //crop it to 3 decimals behind .
        if (timeBehindDot > 999) {
            String time = Long.toString(timeBehindDot);
            String cropped = time.substring(0, 3);
            timeBehindDot = Long.valueOf(cropped);
        }
    }

    public void setAggregateFunctionEnumFromParsedString(final String enumString) {
        switch (enumString) {
            case "MAX" :
                this.selectedFunction = AGGREGATION_FUNCTION.MAX;
                break;
            case "MIN" :
                this.selectedFunction = AGGREGATION_FUNCTION.MIN;
                break;
            case "AVG" :
                this.selectedFunction = AGGREGATION_FUNCTION.AVG;
                break;
            default :
                throw new RuntimeException("Unknown value of string when trying to parse it to AGGREGATE_FUNCTION enum");
        }
    }

    public void setWhichFieldsToSearchThrough(final boolean searchMaxSpeed, final boolean searchPrice, final boolean searchHorsePower,
                                              final boolean searchAge, final boolean searchDoorCount, final boolean searchTrunkVolume,
                                              final boolean searchSpeedometer, final boolean searchTankVolume, final boolean searchOilVolume,
                                              final boolean searchNumberOfSeats) {
        this.searchMaxSpeed = searchMaxSpeed;
        this.searchPrice = searchPrice;
        this.searchHorsePower = searchHorsePower;
        this.searchAge = searchAge;
        this.searchDoorCount = searchDoorCount;
        this.searchTrunkVolume = searchTrunkVolume;
        this.searchSpeedometer = searchSpeedometer;
        this.searchTankVolume = searchTankVolume;
        this.searchOilVolume = searchOilVolume;
        this.searchNumberOfSeats = searchNumberOfSeats;
    }

    //computes and returns aggregate value for given car
    private float getValueForSelectedAggregateFunction(final OrderedColumns.CarWithNormalizedValue car, OrderedColumns orderedColumns) {
        List<Float> normalizedValuesForCarFields = new ArrayList<>();
        if (searchMaxSpeed) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getMaxSpeedMin(),
                    orderedColumns.getMaxSpeedMax(), car.getCar().getMaxSpeed()));
        }
        if (searchPrice) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getPriceMin(),
                    orderedColumns.getPriceMax(), car.getCar().getPrice()));
        }
        if (searchHorsePower) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getHorsePowerMin(),
                    orderedColumns.getHorsePowerMax(), car.getCar().getHorsePower()));
        }
        if (searchAge) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getAgeMin(),
                    orderedColumns.getAgeMax(), car.getCar().getAge()));
        }
        if (searchDoorCount) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getDoorCountMin(),
                    orderedColumns.getDoorCountMax(), car.getCar().getDoorCount()));
        }
        if (searchTrunkVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getTankVolumeMin(),
                    orderedColumns.getTrunkVolumeMax(), car.getCar().getTrunkVolume()));
        }
        if (searchSpeedometer) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getSpeedometerMin(),
                    orderedColumns.getSpeedometerMax(), car.getCar().getSpeedometer()));
        }
        if (searchTankVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getTankVolumeMin(),
                    orderedColumns.getTankVolumeMax(), car.getCar().getTankVolume()));

        }
        if (searchOilVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getOilVolumeMin(),
                    orderedColumns.getOilVolumeMax(), car.getCar().getOilVolume()));
        }
        if (searchNumberOfSeats) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getNumberOfSeatsMin(),
                    orderedColumns.getNumberOfSeatsMax(), car.getCar().getNumberOfSeats()));
        }
        switch (selectedFunction) {
            case AVG:
                double sum = 0;
                int cnt = 0;
                for (Float value : normalizedValuesForCarFields) {
                    sum += value;
                    cnt++;
                }
                return (float)sum/cnt;
            case MAX:
                double max = Double.MIN_VALUE;
                for (Float value : normalizedValuesForCarFields) {
                    if (max < value) {
                        max = value;
                    }
                }
                return (float)max;
            case MIN:
                double min = Double.MAX_VALUE;
                for (Float value : normalizedValuesForCarFields) {
                    if (min > value) {
                        min = value;
                    }
                }
                return (float)min;
            default:
                throw new RuntimeException("Error in selected aggregation function");
        }
    }

    //computes and changes threshold value for the currently viewed objects in parallel
    private void computeThreshold(final OrderedColumns orderedColumns) {
        List<Float> normalizedValues = new ArrayList<>();
        if (searchMaxSpeed) {
            normalizedValues.add(orderedColumns.getMaxSpeedColumn().get(maxSpeedPosition).getNormalizedValue());
        }
        if (searchPrice) {
            normalizedValues.add(orderedColumns.getPriceColumn().get(pricePosition).getNormalizedValue());
        }
        if (searchHorsePower) {
            normalizedValues.add(orderedColumns.getHorsePowerColumn().get(horsePowerPosition).getNormalizedValue());
        }
        if (searchAge) {
            normalizedValues.add(orderedColumns.getAgeColumn().get(agePosition).getNormalizedValue());
        }

        if (searchDoorCount) {
            normalizedValues.add(orderedColumns.getDoorCountColumn().get(doorCountPosition).getNormalizedValue());
        }
        if (searchTrunkVolume) {
            normalizedValues.add(orderedColumns.getTrunkVolumeColumn().get(trunkVolumePosition).getNormalizedValue());
        }
        if (searchSpeedometer) {
            normalizedValues.add(orderedColumns.getSpeedometerColumn().get(speedometerPosition).getNormalizedValue());
        }
        if (searchTankVolume) {
            normalizedValues.add(orderedColumns.getTankVolumeColumn().get(tankVolumePosition).getNormalizedValue());
        }
        if (searchOilVolume) {
            normalizedValues.add(orderedColumns.getOilVolumeColumn().get(oilVolumePosition).getNormalizedValue());
        }
        if (searchNumberOfSeats) {
            normalizedValues.add(orderedColumns.getNumberOfSeatsColumn().get(numberOfSeatsPosition).getNormalizedValue());
        }
        switch (selectedFunction) {
            case AVG:
                thresholdAVG(normalizedValues);
                break;
            case MAX:
                thresholdMAX(normalizedValues);
                break;
            case MIN:
                thresholdMIN(normalizedValues);
                break;
            default:
                throw new RuntimeException("Error in selected aggregation function");
        }
    }

    private void thresholdAVG(final List<Float> normalizedValues) {
        double sum = 0;
        int cnt = 0;
        for (Float value : normalizedValues) {
            sum += value;
            cnt++;
        }
        thresholdValue = (float) sum / cnt;
    }

    private void thresholdMAX(final List<Float> normalizedValues) {
        double max = Double.MIN_VALUE;
        for (Float value : normalizedValues) {
            if (max < value) {
                max = value;
            }
        }
        thresholdValue = (float) max;
    }

    private void thresholdMIN(final List<Float> normalizedValues) {
        double min = Double.MAX_VALUE;
        for (Float value : normalizedValues) {
            if (min > value) {
                min = value;
            }
        }
        thresholdValue = (float) min;
    }

    //finds max normalized value from top of the sorted lists and returns the car with the highest value
    //also increments the counter for that lists position
    private OrderedColumns.CarWithNormalizedValue getBestCarFromTopOfTheColumns(OrderedColumns orderedColumns) {
        float max = -5;
        OrderedColumns.CarWithNormalizedValue carWithBiggestNormalizationValue;

        COLUMN columnWithMaxNormalizationValue = null;

        //nejdriv najdu ten s nejvetsi normalizacni hodnotou a podle toho nastavim enum
        //prvni vec v ifu -> zda podle tohoto sloupce mam vyhledavat nebo ne
        if (searchMaxSpeed && Float.compare(max, orderedColumns.getMaxSpeedColumn().get(maxSpeedPosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getMaxSpeedColumn().get(maxSpeedPosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.MAX_SPEED;
        }

        if (searchPrice && Float.compare(max, orderedColumns.getPriceColumn().get(pricePosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getPriceColumn().get(pricePosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.PRICE;
        }

        if (searchHorsePower && Float.compare(max, orderedColumns.getHorsePowerColumn().get(horsePowerPosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getHorsePowerColumn().get(horsePowerPosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.HORSE_POWER;
        }

        if (searchAge && Float.compare(max, orderedColumns.getAgeColumn().get(agePosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getAgeColumn().get(agePosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.AGE;
        }

        if (searchDoorCount && Float.compare(max, orderedColumns.getDoorCountColumn().get(doorCountPosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getDoorCountColumn().get(doorCountPosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.DOOR_COUNT
            ;
        }

        if (searchTrunkVolume && Float.compare(max, orderedColumns.getTrunkVolumeColumn().get(trunkVolumePosition).getNormalizedValue()) < 0){
            max = orderedColumns.getTrunkVolumeColumn().get(trunkVolumePosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.TRUNK_VOLUME;
        }

        if (searchSpeedometer && Float.compare(max, orderedColumns.getSpeedometerColumn().get(speedometerPosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getSpeedometerColumn().get(speedometerPosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.SPEEDOMETER;
        }

        if (searchTankVolume && Float.compare(max, orderedColumns.getTankVolumeColumn().get(tankVolumePosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getTankVolumeColumn().get(tankVolumePosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.TANK_VOLUME;
        }

        if (searchOilVolume && Float.compare(max, orderedColumns.getOilVolumeColumn().get(oilVolumePosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getOilVolumeColumn().get(oilVolumePosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.OIL_VOLUME;
        }

        if (searchNumberOfSeats && Float.compare(max, orderedColumns.getNumberOfSeatsColumn().get(numberOfSeatsPosition).getNormalizedValue()) < 0) {
            max = orderedColumns.getNumberOfSeatsColumn().get(numberOfSeatsPosition).getNormalizedValue();
            columnWithMaxNormalizationValue = COLUMN.NUMBER_OF_SEATS;
        }

        if (columnWithMaxNormalizationValue == null || max == Float.MIN_VALUE) {
            throw new RuntimeException("Error in searching for the currently best normalization value!");
        }

        switch (columnWithMaxNormalizationValue) {
            case MAX_SPEED:
                carWithBiggestNormalizationValue = orderedColumns.getMaxSpeedColumn().get(maxSpeedPosition);
                maxSpeedPosition++;
                break;
            case PRICE:
                carWithBiggestNormalizationValue = orderedColumns.getPriceColumn().get(pricePosition);
                pricePosition++;
                break;
            case HORSE_POWER:
                carWithBiggestNormalizationValue = orderedColumns.getHorsePowerColumn().get(horsePowerPosition);
                horsePowerPosition++;
                break;
            case AGE:
                carWithBiggestNormalizationValue = orderedColumns.getAgeColumn().get(agePosition);
                agePosition++;
                break;
            case DOOR_COUNT:
                carWithBiggestNormalizationValue = orderedColumns.getDoorCountColumn().get(doorCountPosition);
                doorCountPosition++;
                break;
            case TRUNK_VOLUME:
                carWithBiggestNormalizationValue = orderedColumns.getTrunkVolumeColumn().get(trunkVolumePosition);
                trunkVolumePosition++;
                break;
            case SPEEDOMETER:
                carWithBiggestNormalizationValue = orderedColumns.getSpeedometerColumn().get(speedometerPosition);
                speedometerPosition++;
                break;
            case TANK_VOLUME:
                carWithBiggestNormalizationValue = orderedColumns.getTankVolumeColumn().get(tankVolumePosition);
                tankVolumePosition++;
                break;
            case OIL_VOLUME:
                carWithBiggestNormalizationValue = orderedColumns.getOilVolumeColumn().get(oilVolumePosition);
                oilVolumePosition++;
                break;
            case NUMBER_OF_SEATS:
                carWithBiggestNormalizationValue = orderedColumns.getNumberOfSeatsColumn().get(numberOfSeatsPosition);
                numberOfSeatsPosition++;
                break;
            default:
                throw new RuntimeException("Error in searching for the currently best normalization value!");
        }

        return carWithBiggestNormalizationValue;
    }

    private class AggregationValueComparator implements Comparator<CarWithAppliedAggregationFunction> {
        @Override
        public int compare(CarWithAppliedAggregationFunction c1, CarWithAppliedAggregationFunction c2) {
            return Float.compare(c2.getValue(), c1.getValue());
        }
    }

    public List<Car> getTopKCars() {
        List<Car> extractedCars = new ArrayList<>();
        for (CarWithAppliedAggregationFunction carDto : topKObjects) {
            extractedCars.add(carDto.getCar());
        }
        return extractedCars;
    }

    public long getTimeSec() {
        return timeSec;
    }

    public long getTimeBehindDot() {
        return timeBehindDot;
    }

    class CarWithAppliedAggregationFunction {
        private float value;
        private Car car;

        float getValue() {
            return value;
        }

        void setValue(float value) {
            this.value = value;
        }

        Car getCar() {
            return car;
        }

        void setCar(Car car) {
            this.car = car;
        }
    }
}
