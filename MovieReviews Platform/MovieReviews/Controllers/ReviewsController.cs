using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using MovieReviews.Models;
using MovieReviews.Data;
using System.Linq;

public class ReviewController : Controller
{
    private readonly AppDbContext _context;

    public ReviewController(AppDbContext context)
    {
        _context = context;
    }

 
    public IActionResult ForMovie(int id)
    {
        var movie = _context.Movies
            .Include(m => m.Reviews)
            .FirstOrDefault(m => m.MovieId == id);

        if (movie == null) return NotFound();

        return View(movie); 
    }

    [HttpPost]
    public IActionResult AddReview(int movieId, string reviewerName, string content, float rating)
    {
        if (rating > 10) rating = 10;
        if (rating < 0) rating = 0;

        var review = new Review
        {
            MovieId = movieId,
            ReviewerName = reviewerName,
            Content = content,
            Rating = rating
        };

        _context.Reviews.Add(review);
        _context.SaveChanges();

        return RedirectToAction("ForMovie", new { id = movieId });
    }

    [HttpPost]
    public IActionResult Delete(int id, int movieId)
    {
        var review = _context.Reviews.FirstOrDefault(r => r.ReviewId == id);
        if (review != null)
        {
            _context.Reviews.Remove(review);
            _context.SaveChanges();
        }

        return RedirectToAction("ForMovie", new { id = movieId });
    }


}
