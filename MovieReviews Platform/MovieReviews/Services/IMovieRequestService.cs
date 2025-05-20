using MovieReviews.Models;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public interface IMovieRequestService
    {
        IEnumerable<MovieRequest> GetAllRequests();
        MovieRequest GetRequestById(int id);
        void CreateRequest(MovieRequest request);
        void UpdateRequest(MovieRequest request);
        void DeleteRequest(MovieRequest request);
    }
}
