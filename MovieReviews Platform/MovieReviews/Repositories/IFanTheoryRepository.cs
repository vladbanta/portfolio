using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Repositories
{
    public interface IFanTheoryRepository
    {
        IEnumerable<FanTheory> GetAll();
        FanTheory GetById(int id);
        void Add(FanTheory theory);
        void Update(FanTheory theory);
        void Delete(FanTheory theory);
        IEnumerable<Movie> GetAllMovies();
    }
}
