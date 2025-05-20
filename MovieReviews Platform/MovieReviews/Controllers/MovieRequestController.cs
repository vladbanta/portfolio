using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using MovieReviews.Models;
using MovieReviews.Services;

namespace MovieReviews.Controllers
{
    public class MovieRequestController : Controller
    {
        private readonly IMovieRequestService _service;

        public MovieRequestController(IMovieRequestService service)
        {
            _service = service;
        }

        [HttpGet]
        public IActionResult Index()
        {
            return View(new MovieRequest());
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Index(MovieRequest request)
        {
            if (ModelState.IsValid)
            {
                _service.CreateRequest(request);
                ViewBag.Message = "Movie request sent successfully!";
                return View(new MovieRequest()); // form resetat
            }

            return View(request);
        }

        [Authorize(Roles = "Admin")]
        [HttpGet]
        public IActionResult Requests()
        {
            var requests = _service.GetAllRequests();
            return View(requests);
        }

        [Authorize(Roles = "Admin")]
        [HttpGet]
        public IActionResult Edit(int id)
        {
            var request = _service.GetRequestById(id);
            if (request == null) return NotFound();
            return View(request);
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Edit(MovieRequest request)
        {
            if (ModelState.IsValid)
            {
                _service.UpdateRequest(request);
                return RedirectToAction("Requests");
            }
            return View(request);
        }

        [Authorize(Roles = "Admin")]
        [HttpGet]
        public IActionResult Delete(int id)
        {
            var request = _service.GetRequestById(id);
            if (request != null)
            {
                _service.DeleteRequest(request);
            }
            return RedirectToAction("Requests");
        }

        [HttpGet]
        public IActionResult Details(int id)
        {
            var request = _service.GetRequestById(id);
            if (request == null) return NotFound();

            return View(request);
        }
    }
}
