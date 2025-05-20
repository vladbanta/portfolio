using System;

namespace MovieReviews.Models
{
    public class Review
    {
        public int ReviewId { get; set; }
        public string ReviewerName { get; set; }
        public string Content { get; set; }
        public float Rating { get; set; }

        public int MovieId { get; set; }
        public Movie Movie { get; set; }
    }
}
