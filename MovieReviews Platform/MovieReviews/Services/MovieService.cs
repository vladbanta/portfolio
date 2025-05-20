using MovieReviews.Models;
using MovieReviews.Repositories;
using System.Collections.Generic;

namespace MovieReviews.Services
{
    public class MovieService : IMovieService
    {
        private readonly IMovieRepository _repository;

        public MovieService(IMovieRepository repository)
        {
            _repository = repository;
        }

        public IEnumerable<Movie> GetAllMovies()
        {
            return _repository.GetAll();
        }

        public Movie GetMovieById(int id)
        {
            return _repository.GetById(id);
        }

        public void CreateMovie(Movie movie)
        {
            _repository.Add(movie);
        }

        public void UpdateMovie(Movie movie)
        {
            _repository.Update(movie);
        }

        public void DeleteMovie(Movie movie)
        {
            _repository.Delete(movie);
        }
    }
}
