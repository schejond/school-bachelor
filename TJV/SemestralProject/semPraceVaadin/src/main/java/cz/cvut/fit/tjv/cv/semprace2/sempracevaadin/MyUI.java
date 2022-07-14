package cz.cvut.fit.tjv.cv.semprace2.sempracevaadin;

import javax.servlet.annotation.WebServlet;

import com.vaadin.annotations.Theme;
import com.vaadin.annotations.VaadinServletConfiguration;
import com.vaadin.data.Binder;
import com.vaadin.data.converter.LocalDateTimeToDateConverter;
import com.vaadin.data.converter.StringToIntegerConverter;
import com.vaadin.data.converter.StringToLongConverter;
import com.vaadin.event.selection.SelectionListener;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinServlet;
import com.vaadin.ui.Button;
import com.vaadin.ui.DateTimeField;
import com.vaadin.ui.Grid;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Notification;
import com.vaadin.ui.TextField;
import com.vaadin.ui.UI;
import com.vaadin.ui.VerticalLayout;
import com.vaadin.ui.renderers.ButtonRenderer;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.ContactDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.CustomerDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.OrderDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2restclient.ContactClient;
import cz.cvut.fit.tjv.cv.semprace2.semprace2restclient.CustomerClient;
import cz.cvut.fit.tjv.cv.semprace2.semprace2restclient.OrderClient;
import cz.cvut.fit.tjv.cv.semprace2.semprace2restclient.SemestralkaAppException;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;

@Theme("mytheme")
public class MyUI extends UI {

    //klienti k webove sluzbe
    private final ContactClient contactClient = ContactClient.getINSTANCE();
    private final CustomerClient customerClient = CustomerClient.getINSTANCE();
    private final OrderClient orderClient = OrderClient.getINSTANCE();
    //kolekce - cache dat, aby nebylo nutne volat WS casto
    private List<ContactDTO> contacts;
    private List<CustomerDTO> customers;
    private List<OrderDTO> orders;

    @Override
    protected void init(VaadinRequest vaadinRequest) {
        final VerticalLayout layout = new VerticalLayout();

        //kontrola zda sluzba bezi
        try {
            //preklad z pole na kolekci
            contacts = new ArrayList<>(Arrays.asList(contactClient.retrieveAllJson()));
            customers = new ArrayList<>(Arrays.asList(customerClient.retrieveAllJson()));
            orders = new ArrayList<>(Arrays.asList(orderClient.retrieveAllJson()));
        } catch (SemestralkaAppException e) {
            //sluzba nebezi
            //informujeme uzivatele
            Notification.show("The service is not running" + e.getMessage(), Notification.Type.ERROR_MESSAGE);
            return;
        }
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//contacts
        //hlavicka + tabulka
        Label contactsHead = new Label("Contacts");

        Grid<ContactDTO> contactsGrid = new Grid<>();
        contactsGrid.addColumn(s -> s.getId()).setCaption("Contact ID");
        contactsGrid.addColumn(s -> s.getStreet()).setCaption("Street");
        contactsGrid.addColumn(s -> s.getTown()).setCaption("Town");
        contactsGrid.addColumn(s -> s.getZipCode()).setCaption("Zip Code");
        contactsGrid.addColumn(s -> s.getPhoneNumber()).setCaption("Phone Number");
        contactsGrid.addColumn(s -> "Delete", new ButtonRenderer<>(e -> {
            //zavolani WS
            contactClient.delete(e.getItem().getId());
            //aktualizace kolekce
            contacts.remove(e.getItem());
            //nastaveni zobrazovanych dat v tabulce
            contactsGrid.setItems(contacts);
        }));
        
        //radek na editaci
        Label addContactHead = new Label("Add/edit contact");
        TextField contactId = new TextField("Contact ID");
        TextField street = new TextField("Street");
        TextField town = new TextField("Town");
        TextField zipCode = new TextField("Zip Code");
        TextField phoneNumber = new TextField("Phone Number");
        
        contactsGrid.setItems(contacts);
        
        //binder k radku na editaci
        final Binder<ContactDTO> contactsBinder = new Binder<>(ContactDTO.class);
        contactsBinder.forField(contactId).asRequired().withConverter(new StringToLongConverter("Enter and integer value!")).bind("id");
        contactsBinder.forField(street).bind("street");
        contactsBinder.forField(town).asRequired().bind("town");
        contactsBinder.forField(zipCode).asRequired().withConverter(new StringToIntegerConverter("Enter and integer value!")).bind("zipCode");
        contactsBinder.forField(phoneNumber).withConverter(new StringToLongConverter("Enter and integer value!")).bind("phoneNumber"); 
        contactsGrid.addSelectionListener(e -> {
            if (e.getFirstSelectedItem().isPresent()) {
                contactsBinder.readBean(e.getFirstSelectedItem().get());
            }
        });
        //pridani do jednoho radku
        HorizontalLayout addContactLayout = new HorizontalLayout(contactId, street, town, zipCode, phoneNumber);
        //tlacitko pro submit
        Button submitContact = new Button("Add or edit contact", e -> {
            ContactDTO con = new ContactDTO();
            if (contactsBinder.writeBeanIfValid(con)) {
                contactClient.createOrUpdateJson(con);
                contacts.clear();
                contacts.addAll(Arrays.asList(contactClient.retrieveAllJson()));
                contactsGrid.setItems(contacts);
            }
        });
        layout.addComponents(contactsHead, contactsGrid, addContactHead, addContactLayout, submitContact);
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//customers
        Label customersHead = new Label("Customers");
        
        Grid<CustomerDTO> customersGrid = new Grid<>();
        customersGrid.addColumn(cus -> cus.getId()).setCaption("Customer ID");
        customersGrid.addColumn(cus -> cus.getName()).setCaption("Name");
        customersGrid.addColumn(cus -> cus.getSurname()).setCaption("Surname");
        customersGrid.addColumn(s -> "Delete", new ButtonRenderer<>(e -> {
            //zavolani WS
            customerClient.delete(e.getItem().getId());
            //aktualizace kolekce
            customers.remove(e.getItem());
            //nastaveni zobrazovanych dat v tabulce
            customersGrid.setItems(customers);
        }));
        
        //radek na editaci
        Label addCustomerHead = new Label("Add/edit customer");
        TextField customerId = new TextField("Customer ID");
        TextField firstName = new TextField("First Name");
        TextField surname = new TextField("Surname");
        
        customersGrid.setItems(customers);
        
        
        //binder k radku na editaci
        final Binder<CustomerDTO> customersBinder = new Binder<>(CustomerDTO.class);
        customersBinder.forField(customerId).asRequired().withConverter(new StringToLongConverter("Enter and integer value!")).bind("id");
        customersBinder.forField(firstName).asRequired().bind("name");
        customersBinder.forField(surname).asRequired().bind("surname");
        customersGrid.addSelectionListener(e -> {
            if (e.getFirstSelectedItem().isPresent()) {
                customersBinder.readBean(e.getFirstSelectedItem().get());
            }
        });
        //pridani do jednoho radku
        HorizontalLayout addCustomerLayout = new HorizontalLayout(customerId, firstName, surname);
        
        //tlacitko pro submit
        Button submitCustomer = new Button("Add or edit customer", e -> {
            CustomerDTO con = new CustomerDTO();
            if (customersBinder.writeBeanIfValid(con)) {
                customerClient.createOrUpdateJson(con);
                customers.clear();
                customers.addAll(Arrays.asList(customerClient.retrieveAllJson()));
                customersGrid.setItems(customers);
            }
        });
//--------------------------------------------------------
//tabulka pro kolekci kontaktu
        Grid<ContactDTO> contactsInCustomerGrid = new Grid<>();
        contactsInCustomerGrid.addColumn(s -> s.getId()).setCaption("Contact ID");
        contactsInCustomerGrid.addColumn(s -> s.getStreet()).setCaption("Street");
        contactsInCustomerGrid.addColumn(s -> s.getTown()).setCaption("Town");
        contactsInCustomerGrid.addColumn(s -> s.getZipCode()).setCaption("Zip Code");
        contactsInCustomerGrid.addColumn(s -> s.getPhoneNumber()).setCaption("Phone Number");
        contactsInCustomerGrid.addColumn(s -> "Remove", new ButtonRenderer<>(e -> {
            CustomerDTO selectedCustomer = customersGrid.getSelectedItems().stream().findFirst().get();
            customerClient.removeContactJson(selectedCustomer.getId(), e.getItem().getId(), e.getItem().getStreet(), e.getItem().getTown(), e.getItem().getZipCode(), e.getItem().getPhoneNumber());
            selectedCustomer.getContacts().remove(e.getItem());
            contactsInCustomerGrid.setItems(selectedCustomer.getContacts());
        })).setWidth(100);
        
        //listener pro to, ktery kontakt zobrazovat
        customersGrid.addSelectionListener(event -> {
            Optional<CustomerDTO> selected = event.getFirstSelectedItem();
            if (selected.isPresent()) {
                contactsInCustomerGrid.setItems(selected.get().getContacts());
            } else {
                contactsInCustomerGrid.setItems(new ArrayList<>());
            }
        });
        //tlacitko mezi tabulkami
        Button addContactToCustomerButton = new Button("Add to customer", e -> {
            CustomerDTO selectedCustomer = customersGrid.getSelectedItems().stream().findFirst().get();
            ContactDTO selectedContact = contactsGrid.getSelectedItems().stream().findFirst().get();
            try {
                customerClient.addContactJson(selectedCustomer.getId(), selectedContact.getId(), selectedContact.getStreet(), selectedContact.getTown(), selectedContact.getZipCode(), selectedContact.getPhoneNumber());
                selectedCustomer.getContacts().add(selectedContact);
                contactsInCustomerGrid.setItems(selectedCustomer.getContacts());
            } catch (SemestralkaAppException ex) {
                Notification.show("Cannot add this contact to selected customer", Notification.Type.WARNING_MESSAGE);
            }
        });
        //aktivace a deaktivace tlacitka
        addContactToCustomerButton.setEnabled(false);
        SelectionListener sl1 = e -> addContactToCustomerButton.setEnabled(contactsGrid.getSelectedItems().size() == 1 && customersGrid.getSelectedItems().size() == 1);
        contactsGrid.addSelectionListener(sl1);
        customersGrid.addSelectionListener(sl1);
//--------------------------------------------------------
//tabulka pro kolekci objednavek
        Grid<OrderDTO> ordersInCustomerGrid = new Grid<>();
        ordersInCustomerGrid.addColumn(s -> s.getId()).setCaption("Order ID");
        //ordersInCustomerGrid.addColumn(s -> s.getCustomer()).setCaption("Customer ID");
        ordersInCustomerGrid.addColumn(s -> s.getCreationDate()).setCaption("Creation Date");
        ordersInCustomerGrid.addColumn(s -> s.getPrice()).setCaption("Price");
        ordersInCustomerGrid.addColumn(s -> "Remove", new ButtonRenderer<>(e -> {
        CustomerDTO selectedCustomer = customersGrid.getSelectedItems().stream().findFirst().get();
        try {
            customerClient.removeOrderJson(selectedCustomer.getId(), e.getItem().getId(), e.getItem().getCreationDate(), e.getItem().getPrice());
            selectedCustomer.getOrders().remove(e.getItem());
            ordersInCustomerGrid.setItems(selectedCustomer.getOrders());
        } catch (SemestralkaAppException ex) {
            Notification.show("You can't remove order from this collection!", Notification.Type.WARNING_MESSAGE);
        }
        }));
        //listener pro to, kterou objednavku zobrazovat
        customersGrid.addSelectionListener(event -> {
            Optional<CustomerDTO> selected = event.getFirstSelectedItem();
            if (selected.isPresent()) {
                ordersInCustomerGrid.setItems(selected.get().getOrders());
            } else {
                ordersInCustomerGrid.setItems(new ArrayList<>());
            }
        });
        
        Grid<OrderDTO> ordersGrid = new Grid<>();
        
        //tlacitko mezi tabulkami
        Button addOrderToCustomerButton = new Button("Add to customer", e -> {
            CustomerDTO selectedCustomer = customersGrid.getSelectedItems().stream().findFirst().get();
            OrderDTO selectedOrder = ordersGrid.getSelectedItems().stream().findFirst().get();
            try {
                customerClient.addOrderJson(selectedCustomer.getId(), selectedOrder.getId(), selectedOrder.getCreationDate(), selectedOrder.getPrice());
                selectedCustomer.getOrders().add(selectedOrder);
                ordersInCustomerGrid.setItems(selectedCustomer.getOrders());
            } catch (SemestralkaAppException ex) {
                Notification.show("Cannot add this order to selected customer", Notification.Type.WARNING_MESSAGE);
            }
        });
        //aktivace a deaktivace tlacitka
        addOrderToCustomerButton.setEnabled(false);
        SelectionListener sl2 = e -> addOrderToCustomerButton.setEnabled(ordersGrid.getSelectedItems().size() == 1 && customersGrid.getSelectedItems().size() == 1);
        ordersGrid.addSelectionListener(sl2);
        customersGrid.addSelectionListener(sl2);
        //aby byly tabulky ve stejnem radku
        HorizontalLayout tablesCustomerGrid = new HorizontalLayout(customersGrid, addContactToCustomerButton, contactsInCustomerGrid);
//--------------------------------------------------------
        layout.addComponents(customersHead, tablesCustomerGrid,addOrderToCustomerButton, ordersInCustomerGrid, addCustomerHead, addCustomerLayout, submitCustomer);
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//orders
    //hlavicka + tabulka
        Label ordersHead = new Label("Orders");

//        Grid<OrderDTO> ordersGrid = new Grid<>();
        ordersGrid.addColumn(s -> s.getId()).setCaption("Order ID");
        //ordersGrid.addColumn(s -> s.getCustomer()).setCaption("Customer ID");
        ordersGrid.addColumn(s -> s.getCreationDate()).setCaption("Creation Date");
        ordersGrid.addColumn(s -> s.getPrice()).setCaption("Price");
        ordersGrid.addColumn(s -> "Delete", new ButtonRenderer<>(e -> {
            try {
                orderClient.delete(e.getItem().getId());
                //aktualizace kolekce
                orders.remove(e.getItem());
                //nastaveni zobrazovanych dat v tabulce
                ordersGrid.setItems(orders);
            } catch (SemestralkaAppException ex) {
                Notification.show("You can't remove this order!", Notification.Type.WARNING_MESSAGE);
            }
        }));
        
        //radek na editaci
        Label addOrderHead = new Label("Add/edit order");
        TextField orderId = new TextField("Order ID");
        //TextField orderCustomerId = new TextField("Customer ID");
        DateTimeField creationDate = new DateTimeField("Creation Date");
        TextField price = new TextField("Price");
        
        ordersGrid.setItems(orders);
        
        //binder k radku na editaci
        final Binder<OrderDTO> ordersBinder = new Binder<>(OrderDTO.class);
        ordersBinder.forField(orderId).asRequired().withConverter(new StringToLongConverter("Enter and integer value!")).bind("id");
        //ordersBinder.forField(orderCustomerId).asRequired().withConverter(new StringToLongConverter("Enter and integer value!")).bind("customer");
        ordersBinder.forField(creationDate).asRequired().withConverter(new LocalDateTimeToDateConverter(ZoneId.systemDefault())).bind("creationDate");
        ordersBinder.forField(price).asRequired().withConverter(new StringToLongConverter("Enter and integer value!")).bind("price");
        ordersGrid.addSelectionListener(e -> {
            if (e.getFirstSelectedItem().isPresent()) {
                ordersBinder.readBean(e.getFirstSelectedItem().get());
            }
        });
        //pridani do jednoho radku
        HorizontalLayout addOrderLayout = new HorizontalLayout(orderId, creationDate, price);
        //tlacitko pro submit
        Button submitOrder = new Button("Add or edit order", e -> {
            OrderDTO ord = new OrderDTO();
            if (ordersBinder.writeBeanIfValid(ord)) {
                orderClient.createOrUpdateJson(ord);
                orders.clear();
                orders.addAll(Arrays.asList(orderClient.retrieveAllJson()));
                ordersGrid.setItems(orders);
            }
        });
        layout.addComponents(ordersHead, ordersGrid, addOrderHead, addOrderLayout, submitOrder);

        setContent(layout);
    }

    @WebServlet(urlPatterns = "/*", name = "MyUIServlet", asyncSupported = true)
    @VaadinServletConfiguration(ui = MyUI.class, productionMode = false)
    public static class MyUIServlet extends VaadinServlet {
    }
}
