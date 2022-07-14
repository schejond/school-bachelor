package cz.cvut.fit.tjv.cv.semprace2.semprace2restclient;

import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.ContactDTO;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;

public class ContactClient {
    
    private static final String RESOURCE_PATH = "/contact";
    private static final String RESOURCE_ID_PATH = "{id}";

    private static final ContactClient INSTANCE = new ContactClient();
    
    public ContactClient() {
    }

    public static ContactClient getINSTANCE() {
        return INSTANCE;
    }
    
    public void createOrUpdateJson(ContactDTO c) {
        ContactDTO res = RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).put(Entity.json(c), ContactDTO.class));
        c.setId(res.getId());
        c.setStreet(res.getStreet());
        c.setTown(res.getTown());
        c.setZipCode(res.getZipCode());
        c.setPhoneNumber(res.getPhoneNumber());
    }

    public void createOrUpdateXml(ContactDTO c) {
        RestClient.processRequest(RESOURCE_PATH, t -> t.request().put(Entity.xml(c)));
    }

    public ContactDTO retrieveJson(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_JSON).get(ContactDTO.class));
    }

    public ContactDTO retrieveXml(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_XML).get(ContactDTO.class));
    }

    public ContactDTO[] retrieveAllJson() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).get(ContactDTO[].class));
    }

    public ContactDTO[] retrieveAllXml() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_XML).get(ContactDTO[].class));
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
}
