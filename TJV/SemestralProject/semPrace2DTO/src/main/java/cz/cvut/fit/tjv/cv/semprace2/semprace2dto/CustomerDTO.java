package cz.cvut.fit.tjv.cv.semprace2.semprace2dto;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Objects;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class CustomerDTO implements Serializable {
    private Long id;
    private List<ContactDTO> contacts = new ArrayList<>();
    private List<OrderDTO> orders = new ArrayList<>();
    private String name;
    private String surname;

    public CustomerDTO() {
    }

    //bude mit kolekce, z druhe strany v koonstruktorech nic nebude
    public CustomerDTO(Long id, String name, String surname, List<ContactDTO> contacts, List<OrderDTO> orders) {
        this.id = id;
        this.name = name;
        this.surname = surname;
        this.contacts = contacts;
        this.orders = orders;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public List<ContactDTO> getContacts() {
        return contacts;
    }

    public void setContacts(List<ContactDTO> contacts) {
        this.contacts = contacts;
    }

    public List<OrderDTO> getOrders() {
        return orders;
    }

    public void setOrders(List<OrderDTO> orders) {
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

    @Override
    public int hashCode() {
        int hash = 7;
        hash = 41 * hash + Objects.hashCode(this.id);
        hash = 41 * hash + Objects.hashCode(this.contacts);
        hash = 41 * hash + Objects.hashCode(this.orders);
        hash = 41 * hash + Objects.hashCode(this.name);
        hash = 41 * hash + Objects.hashCode(this.surname);
        return hash;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final CustomerDTO other = (CustomerDTO) obj;
        if (!Objects.equals(this.name, other.name)) {
            return false;
        }
        if (!Objects.equals(this.surname, other.surname)) {
            return false;
        }
        if (!Objects.equals(this.id, other.id)) {
            return false;
        }
        if (!Objects.equals(this.contacts, other.contacts)) {
            return false;
        }
        if (!Objects.equals(this.orders, other.orders)) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "CustomerDTO{" + "id=" + id + ", contacts=" + contacts + ", orders=" + orders + ", name=" + name + ", surname=" + surname + '}';
    }    
    
}
