package Entities;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.OneToMany;
import javax.persistence.PrimaryKeyJoinColumn;
import javax.persistence.Table;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.XmlTransient;
import org.eclipse.persistence.annotations.CascadeOnDelete;

/**
 * @author schejond
 */
@Entity
@Table(name = "Zakaznik")
@XmlRootElement
public class Customer implements Serializable {

    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "zakaznik_ID")
    private Long id;
    
    @CascadeOnDelete
    @ManyToMany(cascade = CascadeType.ALL)
    @JoinTable(name = "zakaznik_adresa",
            joinColumns = @JoinColumn(name = "Zakaznik_zakaznik_ID"),
            inverseJoinColumns = {
                @JoinColumn(name = "Kontakty_udaje_ID")
            })
    private Collection<Contact> contacts = new ArrayList<>();

    public Customer() {
    }

    public Customer(Long id, String name, String surname, Collection<Contact> contacts, Collection<Order> orders) {
        this.id = id;
        this.name = name;
        this.surname = surname;
        this.contacts = contacts;
        this.orders = orders;
    }
    
    @CascadeOnDelete
    @OneToMany(cascade = CascadeType.ALL)//misa u one to many vubec nema mapped by, tak to asi nepis
    @PrimaryKeyJoinColumn(name = "zakaznik_ID", referencedColumnName = "Objednavka_ID")
    private Collection<Order> orders = new ArrayList<>();
    
    @Column(name = "jmeno")
    private String name;
    
    @Column(name = "prijmeni")
    private String surname;

    @XmlTransient
    public Collection<Contact> getContacts() {
        return contacts;
    }

    public void setContacts(Collection<Contact> contacts) {
        this.contacts = contacts;
    }

    @XmlTransient
    public Collection<Order> getOrders() {
        return orders;
    }

    public void setOrders(Collection<Order> orders) {
        this.orders = orders;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSurname() {
        return surname;
    }

    public void setSurname(String surname) {
        this.surname = surname;
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
        if (!(object instanceof Customer)) {
            return false;
        }
        Customer other = (Customer) object;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "Customer{" + "id=" + id + ", contacts=" + contacts + ", orders=" + orders + ", name=" + name + ", surname=" + surname + '}';
    }    
}
