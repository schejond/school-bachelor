package Rest;

import Controllers.AbstractCRUDController;
import Controllers.OrderController;
import Entities.Order;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.OrderDTO;
import java.util.function.Function;
import javax.ejb.EJB;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/order")
public class OrderResource extends AbstractCRUDResource<Order, OrderDTO> {
    static final Function<Order, OrderDTO> ENTITY_TO_DTO_CONVERTER
            = e -> e == null ? null : new OrderDTO(e.getId(), e.getCreationDate(),
                                                     e.getPrice());
    static final Function<OrderDTO, Order> DTO_TO_ENTITY_CONVERTER
            = d -> new Order(d.getId(),d.getCreationDate(), d.getPrice());
    
    @EJB
    private OrderController controller;

    public OrderResource() {
        super(DTO_TO_ENTITY_CONVERTER, ENTITY_TO_DTO_CONVERTER);
    }

    @Override
    protected OrderController getController() {
        return controller;
    }

    @Override
    protected boolean containsEntityId(OrderDTO dto) {
        return dto.getId() != null;
    }
    
    //--
    @GET
    @Path("/{id}")
    @Produces({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Response retrieveById(@PathParam("id") Long id) {
        return super.retrieveById(id);
    }

    @DELETE
    @Path("/{id}")
    @Produces({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Response deleteById(@PathParam("id") Long id) {
        return super.deleteById(id);
    }
    //--
}
