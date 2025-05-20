using MovieReviews.Models;
using MovieReviews.Repositories;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public class ContactMessageService : IContactMessageService
    {
        private readonly IContactMessageRepository _repository;

        public ContactMessageService(IContactMessageRepository repository)
        {
            _repository = repository;
        }

        public IEnumerable<ContactMessage> GetAllMessages()
        {
            return _repository.GetAll();
        }

        public ContactMessage GetMessageById(int id)
        {
            return _repository.GetById(id);
        }

        public void CreateMessage(ContactMessage message)
        {
            _repository.Add(message);
        }

        public void UpdateMessage(ContactMessage message)
        {
            _repository.Update(message);
        }

        public void DeleteMessage(ContactMessage message)
        {
            _repository.Delete(message);
        }
    }
}
