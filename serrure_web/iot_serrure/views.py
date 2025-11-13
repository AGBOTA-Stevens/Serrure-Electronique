from django.shortcuts import render, redirect, get_object_or_404
from .models import Users
from .password_checker import score_password
import random
from django.utils import timezone
from django.http import HttpResponse, JsonResponse
from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.contrib.auth import login, authenticate, logout
from django.core.mail import send_mail, EmailMultiAlternatives
from django.conf import settings
from django.template.loader import render_to_string
from django.contrib.sites.shortcuts import get_current_site
from django.utils.html import strip_tags
from django.utils.encoding import force_bytes, force_str
from django.contrib.auth.tokens import default_token_generator
from django.contrib import messages
from django.utils.http import urlsafe_base64_encode, urlsafe_base64_decode
from django.urls import reverse
from django.contrib.auth.decorators import login_required, user_passes_test



def home(request):
    # Placeholder for home view logic
    try:
        user = request.user
        pk = user.pk
        return render(request, 'home.html', {'user': user, 'pk': pk})
    except:
        user = None
    return render(request, 'home.html')

def connexion(request):
    if request.method == 'POST':
        identifiant = request.POST.get('identifiant')
        password = request.POST.get('password')
        
        user = authenticate(request, identifiant=identifiant, password=password)
        print(user)
        if user is not None:
            login(request, user)
            return redirect(reverse('iot_serrure:dashboard', args=[user.id]))
        elif Users.objects.filter(identifiant=identifiant, password=password).exists():
            user = Users.objects.get(identifiant=identifiant)
            
            login(request, user)
            return redirect(reverse('iot_serrure:dashboard', args=[user.id]))
        else:
            messages.error(request, 'identifiant ou mot de passe incorrect')
            return render(request, 'connexion.html')
    return render(request, 'connexion.html')

@api_view(['POST'])
def verifier_badge_pass(request):
    badge_pass = request.data.get('badge_pass')
    try:
        user = Users.objects.get(badge_pass=badge_pass)
        if user.is_active:
            user.last_login = timezone.now()
            user.badge_pass = None
            user.save()
            return Response({'status': True, 'badge_pass': user.badge_pass})
        else:
            return Response({'status': False, 'message': 'Badge non actif'})
    except Users.DoesNotExist:
        return Response({'status': False})
    except Exception as e:
        return Response({'status': False})

@api_view(['POST'])
def activer_badge_pass(request):
    code = request.data.get('code')
    try:
        user = Users.objects.get(code=code)
        if user is not None:
            return Response({'status': True, 'badge_pass': user.badge_pass})
        else:
            return Response({'status': False, 'message': 'Code invalide'})
    except Users.DoesNotExist:
        return Response({'status': False, 'message': 'Code invalide'})
    except Exception as e:
        return Response({'status': False, 'message': str(e)})
    

@login_required
def dashboard(request, user_id):
    # Placeholder for dashboard view logic
    try:
        user = get_object_or_404(Users, id=user_id)
        if request.method == 'POST':
            user.is_active = request.POST.get('badge') == 'on'
            if 'badge' in request.POST:
                if user.is_active:
                    while True:
                        new_code = str(random.randint(10000, 99999))
                        if not Users.objects.filter(code=new_code).exists():
                            user.code = new_code
                            break
                    user.save()
                    current_site = get_current_site(request)
                    from_email = settings.DEFAULT_FROM_EMAIL
                    to = user.email
                    
                    send_mail(
                        "Code d'accès Serrure Électronique",
                        f"Bonjour M./Mme {user.name} {user.surname}, \nVoici votre code d'accès \n\t {user.code}",
                        from_email,
                        [to],
                        fail_silently=False,
                    )
                messages.success(request, 'Badge mis à jour avec succès')
                return redirect(reverse('iot_serrure:dashboard', args=[user.id]))
            else:
                messages.error(request, 'Veuillez fournir un badge valide')
        return render(request, 'dashboard.html', {'user': user})
    except Users.DoesNotExist:
        messages.error(request, 'Utilisateur non trouvé')
        return redirect(reverse('iot_serrure:home'))
@login_required    
def logout_view(request):
    logout(request)
    return redirect(reverse('iot_serrure:home'))

def password_reset_request(request):
    if request.method == "POST":
        email_identifiant = request.POST.get('email-identifiant')
        associated_users = Users.objects.filter(email=email_identifiant) | Users.objects.filter(identifiant=email_identifiant)
        if associated_users.exists():
            for user in associated_users:
                current_site = get_current_site(request)
                subject = "Réinitialisation de votre mot de passe"
                from_email = settings.DEFAULT_FROM_EMAIL
                to_email = user.email
                uid = urlsafe_base64_encode(force_bytes(user.pk))
                token = default_token_generator.make_token(user)
                reset_url = f"https://{current_site.domain}{reverse('iot_serrure:password_reset_confirm', args=[uid, token])}"
                
                context = {
                    'name': user.name,
                    'surname': user.surname,
                    'reset_url': reset_url,
                    'domain': current_site.domain,
                }
                
                html_content = render_to_string('password_reset_email.html', context)
                text_content = strip_tags(html_content)
                
                email_message = EmailMultiAlternatives(subject, text_content, from_email, [to_email])
                email_message.attach_alternative(html_content, "text/html")
                email_message.send()
            return redirect(reverse('iot_serrure:home'))
        else:
            messages.error(request, 'Aucun utilisateur associé à cette adresse email.')
            return redirect(reverse('iot_serrure:password_reset'))
    return render(request, 'password_reset.html')

def password_reset_confirm(request, uidb64, token):
    try:
        uid = force_str(urlsafe_base64_decode(uidb64))
        user = Users.objects.get(pk=uid)
    except (TypeError, ValueError, OverflowError, Users.DoesNotExist):
        user = None

    if user is not None and default_token_generator.check_token(user, token):
        if request.method == 'POST':
            new_password = request.POST.get('new_password')
            confirm_password = request.POST.get('confirm_password')
            if new_password and new_password == confirm_password:
                result = score_password(new_password)
                if result["score"] < 3:

                    for w in result["warnings"]:
                        messages.warning(request, w)
                    return render(request, 'password_reset_confirm.html', {'validlink': True})
                user.set_password(confirm_password)
                user.save()
                messages.success(request, 'Votre mot de passe a été réinitialisé avec succès.')
                return redirect(reverse('iot_serrure:connexion'))
            else:
                messages.error(request, 'Les mots de passe ne correspondent pas ou sont vides.')
        return render(request, 'password_reset_confirm.html', {'validlink': True})
    else:
        messages.error(request, 'Le lien de réinitialisation est invalide ou a expiré.')
        return redirect(reverse('iot_serrure:connexion'))
    
def handler404(request):
    return render(request, '404.html', status=404)