import {AfterViewInit, Component, OnInit, ViewChild} from '@angular/core';
import {HttpClient, HttpClientJsonpModule} from '@angular/common/http';
import {MatPaginator, MatSort, MatTableDataSource} from '@angular/material';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  private numberOfCarsToGenerate: number;
  private k: number;
  private selectedMaxSpeed: boolean;
  private selectedPrice: boolean;
  private selectedHorsePower: boolean;
  private selectedAge: boolean;
  private selectedDoorCount: boolean;
  private selectedTrunkVolume: boolean;
  private selectedSpeedometer: boolean;
  private selectedTankVolume: boolean;
  private selectedOilVolume: boolean;
  private selectedNumberOfSeats: boolean;
  private selectedFunction: string;
  private thresholdSeconds: number;
  private thresholdTimeAfterDot: number;
  private seqSeconds: number;
  private seqTimeAfterDot: number;
  @ViewChild(MatSort) sort: MatSort;
  dataSource = new MatTableDataSource<Car>();


  @ViewChild(MatPaginator) paginator: MatPaginator;

  elements: Car[] = [];
  displayedColumns = ['id', 'maxSpeed', 'price', 'horsePower',
    'age', 'doorCount', 'trunkVolume', 'speedometer',
    'tankVolume', 'oilVolume', 'numberOfSeats'];
  endpoint = 'http://localhost:4200/api/';
  interval;

  constructor(private http: HttpClient) {
  }

  ngOnInit() {
    this.selectedMaxSpeed = true;
    this.selectedFunction = 'MAX';
    this.dataSource.sort = this.sort;
    this.dataSource.paginator = this.paginator;
    this.interval = setInterval(() => {
      this.dataSource.sort = this.sort;
      this.dataSource.paginator = this.paginator;
    }, 1000);
  }
  onClickMe() {
    this.http.post<Car[]>(this.endpoint + 'cars/insert', {numberOfRecordsToInsert: this.numberOfCarsToGenerate})
      .subscribe(data => {
          // console.log(data);
          console.log('pocet zaznamu: ' + this.numberOfCarsToGenerate);
          this.elements = data;
          this.dataSource.data = data;
          this.dataSource.sort = this.sort;
        },
        error1 => {
          console.log(error1);
        });
  }
  startAlgo() {
    if (typeof this.k === 'undefined' || Number(this.k) <= 0) {
      alert('Value of k has to be greater than 0!');
      return;
    }
    if (Number(this.k) > Number(this.numberOfCarsToGenerate)) {
      alert('Maximum value of k is: ' + this.numberOfCarsToGenerate);
      return;
    }
    if (!this.selectedMaxSpeed && !this.selectedPrice && !this.selectedHorsePower
        && !this.selectedAge && !this.selectedDoorCount && !this.selectedTrunkVolume
        && !this.selectedSpeedometer && !this.selectedTankVolume && !this.selectedOilVolume
        && !this.selectedNumberOfSeats) {
      alert('Choose at least one column!');
      return;
    }
    this.http.post<AlgorithmOutputDto>(this.endpoint + 'cars/startalgo', {
      searchMaxSpeed: this.selectedMaxSpeed,
      searchPrice: this.selectedPrice,
      searchHorsePower: this.selectedHorsePower,
      searchAge: this.selectedAge,
      searchDoorCount: this.selectedDoorCount,
      searchTrunkVolume: this.selectedTrunkVolume,
      searchSpeedometer: this.selectedSpeedometer,
      searchTankVolume: this.selectedTankVolume,
      searchOilVolume: this.selectedOilVolume,
      searchNumberOfSeats: this.selectedNumberOfSeats,
      k: this.k,
      selectedFunction: this.selectedFunction
    }).subscribe(data => {
          this.elements = data.cars;
          this.dataSource.data = data.cars;
          this.dataSource.sort = this.sort;
          this.thresholdSeconds = data.timeSec;
          this.thresholdTimeAfterDot = data.timeAfterDot;
          this.seqSeconds = data.timeSecSeq;
          this.seqTimeAfterDot = data.timeAfterDotSeq;
          console.log('vteriny: ' + data.timeSec);
          console.log('zaTeckou: ' + data.timeAfterDot);
          console.log('vterinySEQ: ' + data.timeSecSeq);
          console.log('zaTeckouSEQ: ' + data.timeAfterDotSeq);
        },
        error1 => {
          console.log(error1);
        });
  }
  returnToData() {
    this.http.get<Car[]>(this.endpoint + 'cars')
      .subscribe(data => {
          // console.log(data);
          this.elements = data;
          this.dataSource.data = data;
          this.dataSource.sort = this.sort;
        },
        error1 => {
          console.log(error1);
        });
    this.seqSeconds = undefined;
    this.seqTimeAfterDot = undefined;
    this.thresholdSeconds = undefined;
    this.thresholdTimeAfterDot = undefined;
  }
}

class Car {
  private id: number;
  private maxSpeed: number;
  private price: number;
  private horsePower: number;
  private age: number;
  private doorCount: number;
  private trunkVolume: number;
  private speedometer: number;
  private tankVolume: number;
  private oilVolume: number;
  private numberOfSeats: number;
}

class AlgorithmOutputDto {
  public cars: Car[];
  public timeSec: number;
  public timeAfterDot: number;
  public timeSecSeq: number;
  public timeAfterDotSeq: number;
}
