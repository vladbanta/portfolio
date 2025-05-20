using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Repositories
{
    public interface IMovieRepository
    {
        IEnumerable<Movie> GetAll();
        Movie GetById(int id);
        void Add(Movie movie);
        void Update(Movie movie);
        void Delete(Movie movie);
    }
}
