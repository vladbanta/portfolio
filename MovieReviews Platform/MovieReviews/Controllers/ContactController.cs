using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using MovieReviews.Models;
using MovieReviews.Services;

namespace MovieReviews.Controllers
{
    public class ContactController : Controller
    {
        private readonly IContactMessageService _service;

        public ContactController(IContactMessageService service)
        {
            _service = service;
        }

        [HttpGet]
        public IActionResult Index()
        {
            return View(new ContactMessage());
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Index(ContactMessage contactMessage)
        {
            if (ModelState.IsValid)
            {
                _service.CreateMessage(contactMessage);
                ViewBag.Message = "Message sent successfully!";
                ModelState.Clear();
            }
            else
            {
                ViewBag.Message = "Form is invalid.";
            }

            return View(new ContactMessage());
        }

        [Authorize(Roles = "Admin")]
        // GET: Contact/Messages
        public IActionResult Messages()
        {
            var messages = _service.GetAllMessages();
            return View(messages);
        }

        [Authorize(Roles = "Admin")]
        // GET: Contact/Details/5
        public IActionResult Details(int id)
        {
            var message = _service.GetMessageById(id);
            if (message == null)
                return NotFound();

            return View(message);
        }

        [Authorize(Roles = "Admin")]
        public IActionResult Delete(int id)
        {
            var message = _service.GetMessageById(id);
            if (message != null)
            {
                _service.DeleteMessage(message);
            }

            return RedirectToAction("Messages");
        }

        [Authorize(Roles = "Admin")]
        // GET: Contact/Edit/5
        public IActionResult Edit(int id)
        {
            var message = _service.GetMessageById(id);
            if (message == null)
                return NotFound();

            return View(message);
        }

        [Authorize(Roles = "Admin")]
        // POST: Contact/Edit/5
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Edit(int id, ContactMessage updatedMessage)
        {
            if (id != updatedMessage.ContactMessageId)
                return NotFound();

            if (ModelState.IsValid)
            {
                _service.UpdateMessage(updatedMessage);
                return RedirectToAction("Messages");
            }

            return View(updatedMessage);
        }

        
    }
}
