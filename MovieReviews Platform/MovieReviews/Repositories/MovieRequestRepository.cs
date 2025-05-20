using MovieReviews.Data;
using MovieReviews.Models;
using System.Collections.Generic;
using System.Linq;

namespace MovieReviews.Repositories
{
    public class MovieRequestRepository : IMovieRequestRepository
    {
        private readonly AppDbContext _context;

        public MovieRequestRepository(AppDbContext context)
        {
            _context = context;
        }

        public IEnumerable<MovieRequest> GetAll()
        {
            return _context.MovieRequests.ToList();
        }

        public MovieRequest GetById(int id)
        {
            return _context.MovieRequests.FirstOrDefault(r => r.MovieRequestId == id);
        }

        public void Add(MovieRequest request)
        {
            _context.MovieRequests.Add(request);
            _context.SaveChanges();
        }

        public void Update(MovieRequest request)
        {
            _context.MovieRequests.Update(request);
            _context.SaveChanges();
        }

        public void Delete(MovieRequest request)
        {
            _context.MovieRequests.Remove(request);
            _context.SaveChanges();
        }
    }
}
