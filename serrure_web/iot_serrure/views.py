from django.shortcuts import render, redirect, get_object_or_404
from .models import Users
import json
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
from django.contrib.auth.decorators import login_required
from time import sleep


def send_badge_pass(request):
    # Placeholder for badge pass sending logic
    return render(request, 'send_badge_pass.html')
# Create your views here.

def home(request):
    # Placeholder for home view logic
    return render(request, 'home.html')

def connexion(request):
    if request.method == 'POST':
        identifiant = request.POST.get('identifiant')
        password = request.POST.get('password')
        user = authenticate(request, identifiant=identifiant, password=password)
        if user is not None:
            login(request, user)
            return redirect(reverse('iot_serrure:home'))
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
            user.save()
            return Response({'status': True, 'message': user})
        else:
            return Response({'status': False, 'message': 'Badge non actif'})
    except Users.DoesNotExist:
        return Response({'status': False, 'message': 'Badge non trouvé'})
    except Exception as e:
        return Response({'status': False, 'message': str(e)})

@api_view(['POST'])
def activer_badge_pass(request, user_id):
    user = get_object_or_404(Users, id=user_id)
    if request.method == 'POST':
        badge_pass = request.POST.get('badge_pass')
        if badge_pass:
            user.badge_pass = badge_pass
            user.is_active = True
            user.save()
            messages.success(request, 'Badge activé avec succès')
            return redirect(reverse('iot_serrure:home'))
        else:
            messages.error(request, 'Veuillez fournir un badge valide')
    return render(request, 'activer_badge_pass.html', {'user': user})