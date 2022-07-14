package Entities;

import Entities.Customer;
import java.util.Date;
import javax.annotation.Generated;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2019-01-08T23:28:23")
@StaticMetamodel(Order.class)
public class Order_ { 

    public static volatile SingularAttribute<Order, Long> price;
    public static volatile SingularAttribute<Order, Long> id;
    public static volatile SingularAttribute<Order, Date> creationDate;
    public static volatile SingularAttribute<Order, Customer> customer;

}