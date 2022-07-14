package vwmschejond;

import logic.OrderedColumns;

public class ThresholdAlgoDto {
    public boolean searchMaxSpeed;
    public boolean searchPrice;
    public boolean searchHorsePower;
    public boolean searchAge;
    public boolean searchDoorCount;
    public boolean searchTrunkVolume;
    public boolean searchSpeedometer;
    public boolean searchTankVolume;
    public boolean searchOilVolume;
    public boolean searchNumberOfSeats;
    public long k;
    public String selectedFunction;

    public boolean isSearchMaxSpeed() {
        return searchMaxSpeed;
    }

    public void setSearchMaxSpeed(boolean searchMaxSpeed) {
        this.searchMaxSpeed = searchMaxSpeed;
    }

    public boolean isSearchPrice() {
        return searchPrice;
    }

    public void setSearchPrice(boolean searchPrice) {
        this.searchPrice = searchPrice;
    }

    public boolean isSearchHorsePower() {
        return searchHorsePower;
    }

    public void setSearchHorsePower(boolean searchHorsePower) {
        this.searchHorsePower = searchHorsePower;
    }

    public boolean isSearchAge() {
        return searchAge;
    }

    public void setSearchAge(boolean searchAge) {
        this.searchAge = searchAge;
    }

    public boolean isSearchDoorCount() {
        return searchDoorCount;
    }

    public void setSearchDoorCount(boolean searchDoorCount) {
        this.searchDoorCount = searchDoorCount;
    }

    public boolean isSearchTrunkVolume() {
        return searchTrunkVolume;
    }

    public void setSearchTrunkVolume(boolean searchTrunkVolume) {
        this.searchTrunkVolume = searchTrunkVolume;
    }

    public boolean isSearchSpeedometer() {
        return searchSpeedometer;
    }

    public void setSearchSpeedometer(boolean searchSpeedometer) {
        this.searchSpeedometer = searchSpeedometer;
    }

    public boolean isSearchTankVolume() {
        return searchTankVolume;
    }

    public void setSearchTankVolume(boolean searchTankVolume) {
        this.searchTankVolume = searchTankVolume;
    }

    public boolean isSearchOilVolume() {
        return searchOilVolume;
    }

    public void setSearchOilVolume(boolean searchOilVolume) {
        this.searchOilVolume = searchOilVolume;
    }

    public boolean isSearchNumberOfSeats() {
        return searchNumberOfSeats;
    }

    public void setSearchNumberOfSeats(boolean searchNumberOfSeats) {
        this.searchNumberOfSeats = searchNumberOfSeats;
    }

    public long getK() {
        return k;
    }

    public void setK(long k) {
        this.k = k;
    }

    public String getSelectedFunction() {
        return selectedFunction;
    }

    public void setSelectedFunction(String selectedFunction) {
        this.selectedFunction = selectedFunction;
    }
}
