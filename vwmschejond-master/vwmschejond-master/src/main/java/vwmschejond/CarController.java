package vwmschejond;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import java.util.List;

@Controller
public class CarController {
    @Autowired
    CarService carService;

    @RequestMapping("/cars")
    @ResponseBody
    public List<Car> getCars() {
        return carService.getCars();
    }

    @RequestMapping("/cars/delete")
    @ResponseBody
    public void deleteCars() {
        carService.deleteAllCars();
    }

    @PostMapping("/cars/insert")
    @ResponseBody
    public List<Car> insertCars(@RequestBody NumberOfRecordsToInsertDto numberOfRecordsToInsert) {
        return carService.insertCars(numberOfRecordsToInsert.getNumberOfRecordsToInsert());
    }

    @PostMapping("/cars/startalgo")
    @ResponseBody
    public AlgorithmOutputDto startThresholdAlgo(@RequestBody ThresholdAlgoDto dto) {
        return carService.startThresholdAlgo(dto.isSearchMaxSpeed(), dto.isSearchPrice(), dto.isSearchHorsePower(),
                dto.isSearchAge(), dto.isSearchDoorCount(), dto.isSearchTrunkVolume(), dto.isSearchSpeedometer(),
                dto.isSearchTankVolume(), dto.isSearchOilVolume(), dto.isSearchNumberOfSeats(), dto.getK(),
                dto.getSelectedFunction());
    }
}
