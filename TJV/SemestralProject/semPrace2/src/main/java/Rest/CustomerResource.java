package Rest;

import Controllers.CustomerController;
import Entities.Customer;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.ContactDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.CustomerDTO;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.OrderDTO;
import java.util.stream.Collectors;
import javax.ejb.EJB;
import javax.ejb.EJBException;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/customer")
public class CustomerResource extends AbstractCRUDResource<Customer, CustomerDTO> {
    @EJB
    private CustomerController controller;

    public CustomerResource() {
        super(d -> new Customer(d.getId(), d.getName(), d.getSurname(),//pak contacty a pak orders
                        d.getContacts().stream()
                         .map(ContactResource.DTO_TO_ENTITY_CONVERTER)
                         .collect(Collectors.toList()),
                        d.getOrders().stream().map(OrderResource.DTO_TO_ENTITY_CONVERTER)
                         .collect(Collectors.toList())
                    )
                ,
                e -> new CustomerDTO(e.getId(), e.getName(), e.getSurname(),
                        e.getContacts()
                        .stream()
                        .map(ContactResource.ENTITY_TO_DTO_CONVERTER)
                        .collect(Collectors.toList()),
                        e.getOrders()
                        .stream()
                        .map(OrderResource.ENTITY_TO_DTO_CONVERTER)
                        .collect(Collectors.toList())
                    )
        );
    }

    @Override
    protected CustomerController getController() {
        return controller;
    }

    @Override
    protected boolean containsEntityId(CustomerDTO dto) {
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

    @POST
    @Path("/ord/{id}")
    @Consumes({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Response addOrRemoveOrder(OrderDTO o, @PathParam("id") Long id, @QueryParam("remove") boolean remove) {
        try {
            if (remove) {
                return Response
                        .ok(entityToDtoConverter.apply(controller.removeOrder(id, OrderResource.DTO_TO_ENTITY_CONVERTER.apply(o))))
                        .build();
            } else {
                return Response
                        .ok(entityToDtoConverter.apply(controller.addOrder(id, OrderResource.DTO_TO_ENTITY_CONVERTER.apply(o))))
                        .build();
            }
        } catch (EJBException exception) {
            return Response.status(Response.Status.CONFLICT).build();
        }
    }
    
    @POST
    @Path("/con/{id}")
    @Consumes({MediaType.APPLICATION_XML, MediaType.APPLICATION_JSON})
    public Response addOrRemoveContact(ContactDTO c, @PathParam("id") Long id, @QueryParam("remove") boolean remove) {
        try {
            if (remove) {
                return Response
                        .ok(entityToDtoConverter.apply(controller.removeContact(id, ContactResource.DTO_TO_ENTITY_CONVERTER.apply(c))))
                        .build();
            } else {
                return Response
                        .ok(entityToDtoConverter.apply(controller.addContact(id, ContactResource.DTO_TO_ENTITY_CONVERTER.apply(c))))
                        .build();
            }
        } catch (EJBException exception) {
            return Response.status(Response.Status.CONFLICT).build();
        }
    }
}
