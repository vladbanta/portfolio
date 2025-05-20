using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public interface IFanTheoryService
    {
        IEnumerable<FanTheory> GetAllTheories();
        FanTheory GetTheoryById(int id);
        void CreateTheory(FanTheory theory);
        void UpdateTheory(FanTheory theory);
        void DeleteTheory(FanTheory theory);
        IEnumerable<Movie> GetAllMovies();
    }
}
