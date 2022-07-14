package vwmschejond;

import java.util.List;

public class AlgorithmOutputDto {
    public List<Car> cars;
    public long timeSec;
    public long timeAfterDot;
    public long timeSecSeq;
    public long timeAfterDotSeq;

    public AlgorithmOutputDto(List<Car> cars, long timeSec, long timeAfterDot, long timeSecSeq, long timeAfterDotSeq) {
        this.cars = cars;
        this.timeSec = timeSec;
        this.timeAfterDot = timeAfterDot;
        this.timeSecSeq = timeSecSeq;
        this.timeAfterDotSeq = timeAfterDotSeq;
    }
}
