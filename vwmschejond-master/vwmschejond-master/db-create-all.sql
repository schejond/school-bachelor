create table cars (
  id                            bigserial not null,
  max_speed                     integer not null,
  price                         integer not null,
  horse_power                   integer not null,
  age                           integer not null,
  door_count                    integer not null,
  trunk_volume                  integer not null,
  speedometer                   integer not null,
  tank_volume                   integer not null,
  oil_volume                    integer not null,
  number_of_seats               integer not null,
  constraint pk_cars primary key (id)
);

