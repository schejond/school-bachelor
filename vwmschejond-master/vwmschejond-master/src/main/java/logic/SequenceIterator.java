package logic;

import vwmschejond.Car;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class SequenceIterator {
    private Threshold.AGGREGATION_FUNCTION selectedFunction = Threshold.AGGREGATION_FUNCTION.MAX;

    private List<CarWithAppliedAggregationFunction> topKObjects = new ArrayList<>();

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

    private long timeSec;
    private long timeBehindDot;

    public SequenceIterator() {
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

    public void setAggregateFunctionEnumFromParsedString(final String enumString) {
        switch (enumString) {
            case "MAX" :
                this.selectedFunction = Threshold.AGGREGATION_FUNCTION.MAX;
                break;
            case "MIN" :
                this.selectedFunction = Threshold.AGGREGATION_FUNCTION.MIN;
                break;
            case "AVG" :
                this.selectedFunction = Threshold.AGGREGATION_FUNCTION.AVG;
                break;
            default :
                throw new RuntimeException("Unknown value of string when trying to parse it to AGGREGATE_FUNCTION enum");
        }
    }

    public void compute(OrderedColumns orderedColumns, final List<Car> cars, final long k) {
        long timeBefore = System.nanoTime();
        for (Car car : cars) {
            CarWithAppliedAggregationFunction carForTopKQueue = new CarWithAppliedAggregationFunction();
            carForTopKQueue.setCar(car);
            carForTopKQueue.setValue(getValueForSelectedAggregateFunction(car, orderedColumns));
            topKObjects.add(carForTopKQueue);
            topKObjects.sort(new AggregationValueComparator());
            long currentTime = System.nanoTime();
            long seconds = (currentTime - timeBefore)/1000000000;
            //if the algo is too slow, stop it
            if (seconds > 10) {
                break;
            }
        }
        topKObjects.sort(new AggregationValueComparator());
        while (topKObjects.size() > k) {
            topKObjects.remove((int)k);
        }
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

    //computes and returns aggregate value for given car
    private float getValueForSelectedAggregateFunction(final Car car, OrderedColumns orderedColumns) {
        List<Float> normalizedValuesForCarFields = new ArrayList<>();
        if (searchMaxSpeed) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getMaxSpeedMin(),
                    orderedColumns.getMaxSpeedMax(), car.getMaxSpeed()));
        }
        if (searchPrice) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getPriceMin(),
                    orderedColumns.getPriceMax(), car.getPrice()));
        }
        if (searchHorsePower) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getHorsePowerMin(),
                    orderedColumns.getHorsePowerMax(), car.getHorsePower()));
        }
        if (searchAge) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getAgeMin(),
                    orderedColumns.getAgeMax(), car.getAge()));
        }
        if (searchDoorCount) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getDoorCountMin(),
                    orderedColumns.getDoorCountMax(), car.getDoorCount()));
        }
        if (searchTrunkVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getTrunkVolumeMin(),
                    orderedColumns.getTrunkVolumeMax(), car.getTrunkVolume()));
        }
        if (searchSpeedometer) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getSpeedometerMin(),
                    orderedColumns.getSpeedometerMax(), car.getSpeedometer()));
        }
        if (searchTankVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getTankVolumeMin(),
                    orderedColumns.getTankVolumeMax(), car.getTankVolume()));

        }
        if (searchOilVolume) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getOilVolumeMin(),
                    orderedColumns.getOilVolumeMax(), car.getOilVolume()));
        }
        if (searchNumberOfSeats) {
            normalizedValuesForCarFields.add(orderedColumns.computeNormalizedValue(orderedColumns.getNumberOfSeatsMin(),
                    orderedColumns.getNumberOfSeatsMax(), car.getNumberOfSeats()));
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
                throw new RuntimeException("Chyba ve vybrane agregacni funkci");
        }
    }

//    public List<Car> getTopKCars() {
//        List<Car> cars = new ArrayList<>();
//        for (CarWithAppliedAggregationFunction car : topKObjects) {
//            cars.add(car.getCar());
//        }
//        return cars;
//    }

    public long getTimeSec() {
        return timeSec;
    }

    public long getTimeBehindDot() {
        return timeBehindDot;
    }

    private class AggregationValueComparator implements Comparator<CarWithAppliedAggregationFunction> {
        @Override
        public int compare(CarWithAppliedAggregationFunction c1, CarWithAppliedAggregationFunction c2) {
            return Float.compare(c2.getValue(), c1.getValue());
        }
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
