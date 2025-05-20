using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using MovieReviews.Models;
using MovieReviews.Services;

namespace MovieReviews.Controllers
{
    public class FanTheoryController : Controller
    {
        private readonly IFanTheoryService _service;

        public FanTheoryController(IFanTheoryService service)
        {
            _service = service;
        }

        public IActionResult Index()
        {
            var theories = _service.GetAllTheories();
            return View(theories);
        }

        [Authorize(Roles = "Admin")]
        public IActionResult Create()
        {
            ViewBag.Movies = _service.GetAllMovies();
            return View();
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Create(FanTheory fanTheory)
        {
            Console.WriteLine("DEBUG -> Title: " + fanTheory.Title);
            Console.WriteLine("DEBUG -> MovieId: " + fanTheory.MovieId);

            if (!ModelState.IsValid)
            {
                Console.WriteLine("DEBUG -> ModelState INVALID");
                foreach (var error in ModelState.Values.SelectMany(v => v.Errors))
                {
                    Console.WriteLine("DEBUG -> Error: " + error.ErrorMessage);
                }

                ViewBag.Movies = _service.GetAllMovies();
                return View(fanTheory);
            }

            Console.WriteLine("DEBUG -> ModelState VALID - Salvăm în DB");
            _service.CreateTheory(fanTheory);
            return RedirectToAction("Index");
        }

        [Authorize(Roles = "Admin")]
        public IActionResult Edit(int id)
        {
            var theory = _service.GetTheoryById(id);
            if (theory == null) return NotFound();

            ViewBag.Movies = _service.GetAllMovies();
            return View(theory);
        }

        [Authorize(Roles = "Admin")]
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Edit(FanTheory fanTheory)
        {
            if (ModelState.IsValid)
            {
                _service.UpdateTheory(fanTheory);
                return RedirectToAction("Index");
            }

            ViewBag.Movies = _service.GetAllMovies();
            return View(fanTheory);
        }

        [Authorize(Roles = "Admin")]
        public IActionResult Delete(int id)
        {
            var theory = _service.GetTheoryById(id);
            if (theory != null)
            {
                _service.DeleteTheory(theory);
            }

            return RedirectToAction("Index");
        }

        public IActionResult Details(int id)
        {
            var theory = _service.GetTheoryById(id);
            if (theory == null) return NotFound();

            return View(theory);
        }
    }
}
