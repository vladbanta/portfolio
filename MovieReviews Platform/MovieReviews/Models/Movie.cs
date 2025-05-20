using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace MovieReviews.Models
{
    public class Movie
    {
        public int MovieId { get; set; }

        [Required]
        public string Title { get; set; } = string.Empty;

        [Required]
        public string Description { get; set; } = string.Empty;

        [Required]
        public string ImageUrl { get; set; } = string.Empty;

        [Range(0, 10)]
        public float Rating { get; set; }

        public ICollection<Review> Reviews { get; set; } = new List<Review>();
        public ICollection<FanTheory> FanTheories { get; set; } = new List<FanTheory>();
        public ICollection<MovieRequest> Requests { get; set; } = new List<MovieRequest>();
    }
}
