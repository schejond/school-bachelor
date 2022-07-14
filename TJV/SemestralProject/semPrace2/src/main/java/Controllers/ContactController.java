package Controllers;

import Entities.Contact;
import javax.ejb.Stateless;

@Stateless
public class ContactController extends AbstractCRUDController<Contact> {

    public ContactController() {
        super(Contact.class);
    }

    @Override
    protected Object getEntityId(Contact e) {
        return e.getId();
    }
    
}
