package Rest;

import Controllers.ContactController;
import Entities.Contact;
import cz.cvut.fit.tjv.cv.semprace2.semprace2dto.ContactDTO;
import java.util.function.Function;
import javax.ejb.EJB;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/contact")
public class ContactResource extends AbstractCRUDResource<Contact, ContactDTO> {
    
    static final Function<Contact, ContactDTO> ENTITY_TO_DTO_CONVERTER
            = e -> e == null ? null : new ContactDTO(e.getId(), e.getTown(), e.getStreet(),
                                                     e.getZipCode(), e.getPhoneNumber());
    static final Function<ContactDTO, Contact> DTO_TO_ENTITY_CONVERTER
            = d -> new Contact(d.getId(), d.getStreet(),d.getTown(), d.getZipCode(),
                                           d.getPhoneNumber());
    
    @EJB
    private ContactController controller;

    public ContactResource() {
        super(DTO_TO_ENTITY_CONVERTER, ENTITY_TO_DTO_CONVERTER);
    }

    @Override
    protected ContactController getController() {
        return controller;
    }

    @Override
    protected boolean containsEntityId(ContactDTO dto) {
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
