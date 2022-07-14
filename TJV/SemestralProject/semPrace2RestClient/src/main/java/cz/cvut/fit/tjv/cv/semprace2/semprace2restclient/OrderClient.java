package cz.cvut.fit.tjv.cv.semprace2.semprace2restclient;

import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.OrderDTO;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;

public class OrderClient {
    private static final String RESOURCE_PATH = "/order";
    private static final String RESOURCE_ID_PATH = "{id}";

    private static final OrderClient INSTANCE = new OrderClient();
    
    public OrderClient() {
    }

    public static OrderClient getINSTANCE() {
        return INSTANCE;
    }
    
    public void createOrUpdateJson(OrderDTO o) {
        OrderDTO res = RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).put(Entity.json(o), OrderDTO.class));
        o.setId(res.getId());
        o.setCustomer(res.getCustomer());
        o.setCreationDate(res.getCreationDate());
        o.setPrice(res.getPrice());
    }

    public void createOrUpdateXml(OrderDTO o) {
        RestClient.processRequest(RESOURCE_PATH, t -> t.request().put(Entity.xml(o)));
    }

    public OrderDTO retrieveJson(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_JSON).get(OrderDTO.class));
    }

    public OrderDTO retrieveXml(Long id) {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.path(RESOURCE_ID_PATH)
                .resolveTemplate("id", id)
                .request(MediaType.APPLICATION_XML).get(OrderDTO.class));
    }

    public OrderDTO[] retrieveAllJson() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_JSON).get(OrderDTO[].class));
    }

    public OrderDTO[] retrieveAllXml() {
        return RestClient.processRequest(RESOURCE_PATH, t -> t.request(MediaType.APPLICATION_XML).get(OrderDTO[].class));
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
