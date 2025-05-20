using MovieReviews.Models;
using MovieReviews.Repositories;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public class FanTheoryService : IFanTheoryService
    {
        private readonly IFanTheoryRepository _repository;

        public FanTheoryService(IFanTheoryRepository repository)
        {
            _repository = repository;
        }

        public IEnumerable<FanTheory> GetAllTheories()
        {
            return _repository.GetAll();
        }

        public FanTheory GetTheoryById(int id)
        {
            return _repository.GetById(id);
        }

        public void CreateTheory(FanTheory theory)
        {
            _repository.Add(theory);
        }

        public void UpdateTheory(FanTheory theory)
        {
            _repository.Update(theory);
        }

        public void DeleteTheory(FanTheory theory)
        {
            _repository.Delete(theory);
        }

        public IEnumerable<Movie> GetAllMovies()
        {
            return _repository.GetAllMovies();
        }
    }
}
