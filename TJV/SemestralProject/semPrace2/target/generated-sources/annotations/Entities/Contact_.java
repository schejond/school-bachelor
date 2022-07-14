package Entities;

import Entities.Customer;
import javax.annotation.Generated;
import javax.persistence.metamodel.CollectionAttribute;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2019-01-08T23:28:23")
@StaticMetamodel(Contact.class)
public class Contact_ { 

    public static volatile SingularAttribute<Contact, Integer> zipCode;
    public static volatile SingularAttribute<Contact, Long> phoneNumber;
    public static volatile SingularAttribute<Contact, String> town;
    public static volatile SingularAttribute<Contact, String> street;
    public static volatile SingularAttribute<Contact, Long> id;
    public static volatile CollectionAttribute<Contact, Customer> customers;

}