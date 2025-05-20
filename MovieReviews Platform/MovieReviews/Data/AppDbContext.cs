using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using MovieReviews.Models;

namespace MovieReviews.Data
{
    public class AppDbContext : IdentityDbContext<ApplicationUser>
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

        public DbSet<Movie> Movies { get; set; }
        public DbSet<Review> Reviews { get; set; }
        public DbSet<FanTheory> FanTheories { get; set; }
        public DbSet<MovieRequest> MovieRequests { get; set; }
        public DbSet<ContactMessage> ContactMessages { get; set; }

        // Removed "User" because Identity handles it
    }
}
