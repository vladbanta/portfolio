using System.ComponentModel.DataAnnotations;

namespace MovieReviews.Models
{
    public class FanTheory
    {
        public int FanTheoryId { get; set; }

        [Required]
        public string Title { get; set; }

        [Required]
        public string Theory { get; set; }

        [Required]
        public string Reason { get; set; }

        [Required]
        public int MovieId { get; set; }

        public Movie? Movie { get; set; }
    }
}
