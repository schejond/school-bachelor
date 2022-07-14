package vwmschejond;

import io.ebean.EbeanServer;
import logic.DataGenerator;
import logic.OrderedColumns;
import logic.SequenceIterator;
import logic.Threshold;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class CarService {
    @Autowired
    EbeanServer server;

    List<Car> carDatabase = new ArrayList<>();

    private OrderedColumns orderedColumns;

    public List<Car> getCars() {
        return carDatabase;
//        return server.find(Car.class).findList();
    }

    public void insertCar(final Car car) {
        car.save();
    }

    public List<Car> insertCars(final long numberOfRecordsToInsert) {
        deleteAllCars();
        carDatabase = DataGenerator.generateCars(numberOfRecordsToInsert);
//        List<Car> cars = DataGenerator.generateCars(numberOfRecordsToInsert);
//        for (Car car : cars) {
//            insertCar(car);
//        }
        orderedColumns = new OrderedColumns();
//        orderedColumns.precompute(server.find(Car.class).findList());
        orderedColumns.precompute(carDatabase);
        return getCars();
    }

    public void deleteAllCars() {
        carDatabase.clear();
//        server.find(Car.class).delete();
    }

    public AlgorithmOutputDto startThresholdAlgo(final boolean searchMaxSpeed, final boolean searchPrice,
                                   final boolean searchHorsePower, final boolean searchAge,
                                   final boolean searchDoorCount, final boolean searchTrunkVolume,
                                   final boolean searchSpeedometer, final boolean searchTankVolume,
                                   final boolean searchOilVolume, final boolean searchNumberOfSeats,
                                   final long k, final String selectedFunction) {
        for (Car car : carDatabase) {
            car.setInTopList(false);
        }
        //threshold
        Threshold thr = new Threshold();
        thr.setAggregateFunctionEnumFromParsedString(selectedFunction);
        thr.setWhichFieldsToSearchThrough(searchMaxSpeed, searchPrice, searchHorsePower, searchAge, searchDoorCount,
                searchTrunkVolume, searchSpeedometer, searchTankVolume, searchOilVolume, searchNumberOfSeats);
        thr.compute(orderedColumns, k);
        List<Car> cars = thr.getTopKCars();
        System.out.println("Threshold algo evaluation time: " + thr.getTimeSec() + "." + thr.getTimeBehindDot() + " sec");
        //seq
        SequenceIterator seq = new SequenceIterator();
        seq.setAggregateFunctionEnumFromParsedString(selectedFunction);
        seq.setWhichFieldsToSearchThrough(searchMaxSpeed, searchPrice, searchHorsePower, searchAge, searchDoorCount,
                searchTrunkVolume, searchSpeedometer, searchTankVolume, searchOilVolume, searchNumberOfSeats);
        seq.compute(orderedColumns, this.getCars(), k);
        System.out.println("Seq algo evaluation time: " + seq.getTimeSec() + "." + seq.getTimeBehindDot() + " sec");
        return new AlgorithmOutputDto(cars, thr.getTimeSec(), thr.getTimeBehindDot(), seq.getTimeSec(), seq.getTimeBehindDot());
    }

}
