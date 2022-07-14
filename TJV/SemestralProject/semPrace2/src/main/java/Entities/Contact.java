package Entities;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToMany;
import javax.persistence.Table;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;

/**
 * @author schejond
 */
@Entity
@Table(name = "Kontakty")
@XmlRootElement
public class Contact implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "udaje_ID")
    private Long id;

    public Contact() {
    }

    public Contact(Long id, String street, String town, Integer zipCode, Long phoneNumber) {
        this.id = id;
        this.street = street;
        this.town = town;
        this.zipCode = zipCode;
        this.phoneNumber = phoneNumber;
    }
    
    //tady u te nema byt v konstruktoru tento paramaetr, u druhe casti tohoto spojeni ma byt
    @ManyToMany(mappedBy = "contacts")
    private Collection<Customer> customers = new ArrayList<>();
    
    @Column(name = "ulice")
    private String street;
    
    @Column(name = "mesto")
    private String town;
    
    @Column(name = "PSC")
    private Integer zipCode;
    
    @Column(name = "telefonni_cislo")
    private Long phoneNumber;

    //@XmlTransient ??
    public Collection<Customer> getCustomers() {
        return customers;
    }

    public void setCustomers(Collection<Customer> customers) {
        this.customers = customers;
    }

    public String getStreet() {
        return street;
    }

    public void setStreet(String street) {
        this.street = street;
    }

    public String getTown() {
        return town;
    }

    public void setTown(String town) {
        this.town = town;
    }

    public Integer getZipCode() {
        return zipCode;
    }

    public void setZipCode(Integer zipCode) {
        this.zipCode = zipCode;
    }

    public Long getPhoneNumber() {
        return phoneNumber;
    }

    public void setPhoneNumber(Long phoneNumber) {
        this.phoneNumber = phoneNumber;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (id != null ? id.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Contact)) {
            return false;
        }
        Contact other = (Contact) object;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "Contact{" + "id=" + id + ", customers=" + customers + ", street=" + street + ", town=" + town + ", zipCode=" + zipCode + ", phoneNumber=" + phoneNumber + '}';
    }    
}
