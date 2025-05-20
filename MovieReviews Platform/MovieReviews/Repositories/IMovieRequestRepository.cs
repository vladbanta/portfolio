using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Repositories
{
    public interface IMovieRequestRepository
    {
        IEnumerable<MovieRequest> GetAll();
        MovieRequest GetById(int id);
        void Add(MovieRequest request);
        void Update(MovieRequest request);
        void Delete(MovieRequest request);
    }
}
