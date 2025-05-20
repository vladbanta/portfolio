using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Repositories
{
    public interface IContactMessageRepository
    {
        IEnumerable<ContactMessage> GetAll();
        ContactMessage GetById(int id);
        void Add(ContactMessage message);
        void Update(ContactMessage message);
        void Delete(ContactMessage message);
    }
}
