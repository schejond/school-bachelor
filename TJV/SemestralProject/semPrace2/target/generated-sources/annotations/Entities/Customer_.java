package Entities;

import Entities.Contact;
import Entities.Order;
import javax.annotation.Generated;
import javax.persistence.metamodel.CollectionAttribute;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2019-01-08T23:28:23")
@StaticMetamodel(Customer.class)
public class Customer_ { 

    public static volatile SingularAttribute<Customer, String> surname;
    public static volatile SingularAttribute<Customer, String> name;
    public static volatile CollectionAttribute<Customer, Order> orders;
    public static volatile SingularAttribute<Customer, Long> id;
    public static volatile CollectionAttribute<Customer, Contact> contacts;

}