using MovieReviews.Models;
using MovieReviews.Repositories;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public class MovieRequestService : IMovieRequestService
    {
        private readonly IMovieRequestRepository _repository;

        public MovieRequestService(IMovieRequestRepository repository)
        {
            _repository = repository;
        }

        public IEnumerable<MovieRequest> GetAllRequests()
        {
            return _repository.GetAll();
        }

        public MovieRequest GetRequestById(int id)
        {
            return _repository.GetById(id);
        }

        public void CreateRequest(MovieRequest request)
        {
            _repository.Add(request);
        }

        public void UpdateRequest(MovieRequest request)
        {
            _repository.Update(request);
        }

        public void DeleteRequest(MovieRequest request)
        {
            _repository.Delete(request);
        }
    }
}
