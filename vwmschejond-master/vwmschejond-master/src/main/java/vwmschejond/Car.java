package vwmschejond;

import javax.persistence.Entity;
import javax.persistence.Table;

@Entity
@Table(name = "cars")
public class Car extends BaseModel {

    private int maxSpeed;
    private int price;
    private int horsePower;
    private int age;
    private int doorCount;
    private int trunkVolume;
    private int speedometer;
    private int tankVolume;
    private int oilVolume;
    private int numberOfSeats;
    private boolean inTopList;

    public Car() {
        super();
    }

    public Car(Integer maxSpeed, int price, int horsePower, int age, int doorCount,
               int trunkVolume, int speedometer, int tankVolume, int oilVolume, int numberOfSeats) {
        super();
        this.maxSpeed = maxSpeed;
        this.price = price;
        this.horsePower = horsePower;
        this.age = age;
        this.doorCount = doorCount;
        this.trunkVolume = trunkVolume;
        this.speedometer = speedometer;
        this.tankVolume = tankVolume;
        this.oilVolume = oilVolume;
        this.numberOfSeats = numberOfSeats;
        this.inTopList = false;
    }

    public Integer getMaxSpeed() {
        return maxSpeed;
    }

    public void setMaxSpeed(Integer maxSpeed) {
        this.maxSpeed = maxSpeed;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public int getHorsePower() {
        return horsePower;
    }

    public void setHorsePower(int horsePower) {
        this.horsePower = horsePower;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public int getDoorCount() {
        return doorCount;
    }

    public void setDoorCount(int doorCount) {
        this.doorCount = doorCount;
    }

    public int getTrunkVolume() {
        return trunkVolume;
    }

    public void setTrunkVolume(int trunkVolume) {
        this.trunkVolume = trunkVolume;
    }

    public int getSpeedometer() {
        return speedometer;
    }

    public void setSpeedometer(int speedometer) {
        this.speedometer = speedometer;
    }

    public int getTankVolume() {
        return tankVolume;
    }

    public void setTankVolume(int tankVolume) {
        this.tankVolume = tankVolume;
    }

    public int getOilVolume() {
        return oilVolume;
    }

    public void setOilVolume(int oilVolume) {
        this.oilVolume = oilVolume;
    }

    public int getNumberOfSeats() {
        return numberOfSeats;
    }

    public void setNumberOfSeats(int numberOfSeats) {
        this.numberOfSeats = numberOfSeats;
    }

    public boolean isInTopList() {
        return inTopList;
    }

    public void setInTopList(boolean inTopList) {
        this.inTopList = inTopList;
    }
}
