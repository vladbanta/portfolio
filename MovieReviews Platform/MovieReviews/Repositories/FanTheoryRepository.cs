using MovieReviews.Data;
using MovieReviews.Models;
using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;
using System.Linq;

namespace MovieReviews.Repositories
{
    public class FanTheoryRepository : IFanTheoryRepository
    {
        private readonly AppDbContext _context;

        public FanTheoryRepository(AppDbContext context)
        {
            _context = context;
        }

        public IEnumerable<FanTheory> GetAll()
        {
            return _context.FanTheories.Include(ft => ft.Movie).ToList();
        }

        public FanTheory GetById(int id)
        {
            return _context.FanTheories.Include(ft => ft.Movie).FirstOrDefault(ft => ft.FanTheoryId == id);
        }

        public void Add(FanTheory theory)
        {
            _context.FanTheories.Add(theory);
            _context.SaveChanges();
        }

        public void Update(FanTheory theory)
        {
            _context.FanTheories.Update(theory);
            _context.SaveChanges();
        }

        public void Delete(FanTheory theory)
        {
            _context.FanTheories.Remove(theory);
            _context.SaveChanges();
        }

        public IEnumerable<Movie> GetAllMovies()
        {
            return _context.Movies.ToList();
        }
    }
}
