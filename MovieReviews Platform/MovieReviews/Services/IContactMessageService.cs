using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public interface IContactMessageService
    {
        IEnumerable<ContactMessage> GetAllMessages();
        ContactMessage GetMessageById(int id);
        void CreateMessage(ContactMessage message);
        void UpdateMessage(ContactMessage message);
        void DeleteMessage(ContactMessage message);
    }
}
