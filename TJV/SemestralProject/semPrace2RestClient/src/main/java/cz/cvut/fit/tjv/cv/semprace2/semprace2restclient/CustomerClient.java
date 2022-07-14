package cz.cvut.fit.tjv.cv.semprace2.semprace2restclient;

import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.ContactDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.CustomerDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.OrderDTO;
import java.util.Date;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;

public class CustomerClient {
    
    private static final String RESOURCE_PATH = "/customer";
    private static final String RESOURCE_ID_PATH = "{id}";

    private static final CustomerClient INSTANCE = new CustomerClient();
    
    public CustomerClient() {
    }

    public static CustomerClient getINSTANCE() {
        return INSTANCE;
    }  
    
    public void createOrUpdateJson(CustomerDTO c) {
        CustomerDTO res = RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).put(Entity.json(c), CustomerDTO.class));
        c.setId(res.getId());
        c.setName(res.getName());
        c.setSurname(res.getSurname());
        c.setContacts(res.getContacts());
        c.setOrders(res.getOrders());
    }

    public void createOrUpdateXml(CustomerDTO c) {
        RestClient.processRequest(RESOURCE_PATH, t -> t.request().put(Entity.xml(c)));
    }

    public CustomerDTO retrieveJson(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_JSON).get(CustomerDTO.class));
    }

    public CustomerDTO retrieveXml(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_XML).get(CustomerDTO.class));
    }

    public CustomerDTO[] retrieveAllJson() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).get(CustomerDTO[].class));
    }

    public CustomerDTO[] retrieveAllXml() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_XML).get(CustomerDTO[].class));
    }

    public void delete(Long id) {
        RestClient.processRequest(RESOURCE_PATH,
                t -> {
                    int status = t.path(RESOURCE_ID_PATH).resolveTemplate("id", id).request().delete().getStatus();
                    if (status != 204) {
                        throw new SemestralkaAppException(null);
                    } else {
                        return null;
                    }
                });
    }

    private void addOrRemoveContactJson(Long idCus, Long idCon, String street, String town, Integer zipCode, Long phoneNumber, boolean remove) {
        RestClient.processRequest(RESOURCE_PATH + "/con", t -> {
            int status = t.path(RESOURCE_ID_PATH).resolveTemplate("id", idCus).queryParam("remove", remove).request().post(Entity.json(new ContactDTO(idCon, street, town, zipCode, phoneNumber))).getStatus();
            if (status != 200 && status != 204)
                throw new SemestralkaAppException(null);
            else
                return null;
        });
    }

    public void addContactJson(Long idCus, Long idCon, String street, String town, Integer zipCode, Long phoneNumber) {
        addOrRemoveContactJson(idCus, idCon, street, town, zipCode, phoneNumber, false);
    }

    public void removeContactJson(Long idCus, Long idCon, String street, String town, Integer zipCode, Long phoneNumber) {
        addOrRemoveContactJson(idCus, idCon, street, town, zipCode, phoneNumber, true);
    }
    
    private void addOrRemoveOrderJson(Long idCus, Long idOrd, Date creationDate, Long price, boolean remove) {
        RestClient.processRequest(RESOURCE_PATH + "/ord", t -> {
            int status = t.path(RESOURCE_ID_PATH).resolveTemplate("id", idCus).queryParam("remove", remove).request().post(Entity.json(new OrderDTO(idOrd, creationDate, price))).getStatus();
            if (status != 200 && status != 204)
                throw new SemestralkaAppException(null);
            else
                return null;
        });
    }
    
    public void addOrderJson(Long idCus, Long idOrd, Date creationDate, Long price) {
        addOrRemoveOrderJson(idCus, idOrd, creationDate, price, false);
    }

    public void removeOrderJson(Long idCus, Long idOrd, Date creationDate, Long price) {
        addOrRemoveOrderJson(idCus, idOrd, creationDate, price, true);
    }
}
