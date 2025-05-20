using System.ComponentModel.DataAnnotations;

namespace MovieReviews.Models
{
    public class MovieRequest
    {
        public int MovieRequestId { get; set; }

        [Required]
        public string Name { get; set; }

        [Required]
        [EmailAddress]
        public string Email { get; set; }

        [Required]
        public string MovieTitle { get; set; }

        [Required]
        public string Reason { get; set; }

        public int? MovieId { get; set; } 
        public Movie? Movie { get; set; }
    }
}
