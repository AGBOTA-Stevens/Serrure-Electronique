from .models import Users
from django.db.models.signals import post_save
from django.dispatch import receiver
from django.core.mail import EmailMultiAlternatives
from django.template.loader import render_to_string
from django.contrib.sites.shortcuts import get_current_site
from django.conf import settings
from django.contrib.auth.tokens import default_token_generator
from django.utils.encoding import force_bytes
from django.utils.html import strip_tags
from django.utils.http import urlsafe_base64_encode, urlsafe_base64_decode

@receiver(post_save, sender=Users)
def send_welcome_email(sender, instance, created, **kwargs):
    if created:
        subject = 'Bienvenue sur IOT Serrure'
        from_email = settings.DEFAULT_FROM_EMAIL
        current_site = 'iot-serrure.loca.lt'
        to_email = instance.email
        uid = urlsafe_base64_encode(force_bytes(instance.pk))
        
        context = {
            'name': instance.name,
            'surname': instance.surname,
            'identifiant': instance.identifiant,
            'domain': current_site,
            'password': instance.password,
            'uid': uid,
            'token': default_token_generator.make_token(instance),
        }
        
        html_content = render_to_string('welcome_email.html', context)
        text_content = strip_tags(html_content)
        
        email = EmailMultiAlternatives(subject, text_content, from_email, [to_email])
        email.attach_alternative(html_content, "text/html")
        email.send()