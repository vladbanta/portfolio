using MovieReviews.Data;
using MovieReviews.Models;
using System.Collections.Generic;
using System.Linq;

namespace MovieReviews.Repositories
{
    public class ContactMessageRepository : IContactMessageRepository
    {
        private readonly AppDbContext _context;

        public ContactMessageRepository(AppDbContext context)
        {
            _context = context;
        }

        public IEnumerable<ContactMessage> GetAll()
        {
            return _context.ContactMessages.ToList();
        }

        public ContactMessage GetById(int id)
        {
            return _context.ContactMessages.FirstOrDefault(m => m.ContactMessageId == id);
        }

        public void Add(ContactMessage message)
        {
            _context.ContactMessages.Add(message);
            _context.SaveChanges();
        }

        public void Update(ContactMessage message)
        {
            _context.ContactMessages.Update(message);
            _context.SaveChanges();
        }

        public void Delete(ContactMessage message)
        {
            _context.ContactMessages.Remove(message);
            _context.SaveChanges();
        }
    }
}
