using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using MovieReviews.Models;
using MovieReviews.ViewModels;
using System.Threading.Tasks;

namespace MovieReviews.Controllers
{
    public class AccountController : Controller
    {
        private readonly UserManager<ApplicationUser> _userManager;
        private readonly SignInManager<ApplicationUser> _signInManager;
        private readonly RoleManager<IdentityRole> _roleManager;

        public AccountController(UserManager<ApplicationUser> userManager,
                                 SignInManager<ApplicationUser> signInManager,
                                 RoleManager<IdentityRole> roleManager)
        {
            _userManager = userManager;
            _signInManager = signInManager;
            _roleManager = roleManager;
        }

        // ✅ LOGIN
        [HttpGet]
        public IActionResult Login() => View();

        [HttpPost]
        public async Task<IActionResult> Login(LoginViewModel model)
        {
            if (!ModelState.IsValid) return View(model);

            var result = await _signInManager.PasswordSignInAsync(model.Email, model.Password, model.RememberMe, false);

            if (result.Succeeded)
            {
                // ✅ Verificăm dacă este Admin
                var user = await _userManager.FindByEmailAsync(model.Email);
                if (await _userManager.IsInRoleAsync(user, "Admin"))
                {
                    TempData["Message"] = "Welcome, Admin!";
                    return Redirect("~/Home/Index");

                }

                // ✅ Dacă este User simplu
                TempData["Message"] = "Login successful! Welcome back!";
                return Redirect("~/Home/Index");

            }

            ModelState.AddModelError("", "Invalid login attempt");
            return View(model);
        }

        // ✅ REGISTER
        [HttpGet]
        public IActionResult Register() => View();


        [HttpPost]
        public async Task<IActionResult> Register(RegisterViewModel model)
        {
            Console.WriteLine("📌 Register called");  // ✅ LOG ADDED

            if (!ModelState.IsValid)
            {
                Console.WriteLine("❌ ModelState is not valid");
                return View(model);
            }

            // ✅ Verificăm dacă email-ul există deja
            var existingUser = await _userManager.FindByEmailAsync(model.Email);
            if (existingUser != null)
            {
                Console.WriteLine("❌ Email already registered.");
                ModelState.AddModelError("", "Email already registered.");
                TempData["Message"] = "Email already registered!";
                return View(model);
            }

            Console.WriteLine("✅ Creating user...");
            var user = new ApplicationUser { UserName = model.Email, Email = model.Email };
            var result = await _userManager.CreateAsync(user, model.Password);

            if (result.Succeeded)
            {
                Console.WriteLine("✅ User created successfully.");

                if (!await _roleManager.RoleExistsAsync("User"))
                {
                    await _roleManager.CreateAsync(new IdentityRole("User"));
                }

                await _userManager.AddToRoleAsync(user, "User");
                await _signInManager.SignInAsync(user, isPersistent: false);

                TempData["Message"] = "Registration successful! Welcome!";
                return Redirect("~/Home/Index");

            }

            Console.WriteLine("❌ Registration failed.");
            foreach (var error in result.Errors)
            {
                Console.WriteLine($"❌ Error: {error.Description}");
                ModelState.AddModelError("", error.Description);

            }

            return View(model);
        }


        // ✅ ACCESS DENIED
        [HttpGet]
        public IActionResult AccessDenied()
        {
            return View();
        }

        // ✅ LOGOUT
        public async Task<IActionResult> Logout()
        {
            await _signInManager.SignOutAsync();
            TempData["Message"] = "You have been logged out.";
            return Redirect("~/Home/Index");

        }
    }
}
